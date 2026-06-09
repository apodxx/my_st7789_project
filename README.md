# ST7789 240×240 LCD 显示示例

基于 **ESP-IDF** 的 ESP32-S3 项目，驱动 **ST7789** 彩色 TFT 屏（240×240），并通过 **LVGL** 显示图形界面，支持中英文混排。

## 功能

- SPI 驱动 ST7789 面板（RGB565，16 位色深）
- 集成 `esp_lvgl_port`，使用 LVGL 9 构建 UI
- 演示界面：黑底白字，居中显示「你好，中文显示 / Hello ESP32」

## 硬件要求

| 项目 | 说明 |
|------|------|
| 主控 | ESP32-S3 |
| 屏幕 | ST7789，240×240 像素 |
| 接口 | SPI（4 线：SCLK、MOSI、DC、CS）+ RST、背光 |

### 引脚连接

| 屏幕引脚 | ESP32-S3 GPIO | 说明 |
|----------|---------------|------|
| SCL / SCLK | GPIO 21 | SPI 时钟 |
| SDA / MOSI | GPIO 47 | SPI 数据 |
| RES / RST | GPIO 45 | 复位 |
| DC | GPIO 40 | 数据/命令选择 |
| CS | GPIO 41 | 片选 |
| BLK | GPIO 42 | 背光（高电平点亮） |

> 引脚定义见 [`main/board_config.h`](main/board_config.h)，可按实际接线修改。

## 软件环境

- [ESP-IDF](https://docs.espressif.com/projects/esp-idf/) **v5.2** 或更高版本
- 目标芯片：`esp32s3`（已在 `sdkconfig` 中配置）

主要依赖（由 Component Manager 自动拉取）：

- `espressif/esp_lvgl_port`
- `lvgl/lvgl`（v9.x）

## 快速开始

```bash
# 1. 加载 ESP-IDF 环境（路径按本机安装位置调整）
. $HOME/esp/esp-idf/export.sh        # Linux / macOS
# 或 Windows: d:\esp\v5.2.7\esp-idf\export.ps1

# 2. 设置目标并编译
idf.py set-target esp32s3
idf.py build

# 3. 烧录并查看串口日志（将 COM 口改为实际端口）
idf.py -p COM5 flash monitor
```

使用 VS Code + ESP-IDF 插件时，可直接点击 **Build / Flash / Monitor**。

## 项目结构

```
my_st7789_project/
├── main/
│   ├── board_config.h   # 引脚与屏幕参数
│   ├── display.c/h      # SPI、ST7789、LVGL 显示初始化
│   ├── ui.c/h           # LVGL 界面
│   └── main.c           # 程序入口
├── CMakeLists.txt
├── sdkconfig
└── README.md
```

## 自定义 UI

在 [`main/ui.c`](main/ui.c) 的 `ui_create_demo_screen()` 中修改 LVGL 控件；新增界面逻辑可继续放在 `ui.c`，或新建模块并在 `main.c` 中调用。

显示驱动相关改动（分辨率、颜色顺序、镜像等）集中在 [`main/display.c`](main/display.c)。

## 许可证

示例代码，可自由修改用于个人或商业项目。
