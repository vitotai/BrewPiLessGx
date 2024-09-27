#pragma once

#define LGFX_USE_V1

#include <LovyanGFX.hpp>
#include "lv_drv_conf.h"

namespace lgfx
{
 inline namespace v1
 {
//----------------------------------------------------------------------------

  struct Panel_ILI9341m : public Panel_LCD
  {
    Panel_ILI9341m(void)
    {
      _cfg.memory_width  = _cfg.panel_width  = 240;
      _cfg.memory_height = _cfg.panel_height = 320;
    }

  protected:

    static constexpr uint8_t CMD_FRMCTR1 = 0xB1;
    static constexpr uint8_t CMD_FRMCTR2 = 0xB2;
    static constexpr uint8_t CMD_FRMCTR3 = 0xB3;
    static constexpr uint8_t CMD_INVCTR  = 0xB4;
    static constexpr uint8_t CMD_DFUNCTR = 0xB6;
    static constexpr uint8_t CMD_PWCTR1  = 0xC0;
    static constexpr uint8_t CMD_PWCTR2  = 0xC1;
    static constexpr uint8_t CMD_PWCTR3  = 0xC2;
    static constexpr uint8_t CMD_PWCTR4  = 0xC3;
    static constexpr uint8_t CMD_PWCTR5  = 0xC4;
    static constexpr uint8_t CMD_VMCTR1  = 0xC5;
    static constexpr uint8_t CMD_VMCTR2  = 0xC7;
    static constexpr uint8_t CMD_GMCTRP1 = 0xE0; // Positive Gamma Correction (E0h)
    static constexpr uint8_t CMD_GMCTRN1 = 0xE1; // Negative Gamma Correction (E1h)
    static constexpr uint8_t CMD_RDINDEX = 0xD9; // ili9341
    static constexpr uint8_t CMD_IDXRD   = 0xDD; // ILI9341 only, indexed control register read

    const uint8_t* getInitCommands(uint8_t listno) const override
    {
      static constexpr uint8_t list0[] =
      {
          0xEF       , 3, 0x03,0x80,0x02,
          0xCF       , 3, 0x00,0xC1,0x30,
          0xED       , 4, 0x64,0x03,0x12,0x81,
          0xE8       , 3, 0x85,0x00,0x78,
          0xCB       , 5, 0x39,0x2C,0x00,0x34,0x02,
          0xF7       , 1, 0x20,
          0xEA       , 2, 0x00,0x00,
          CMD_PWCTR1,  1, 0x23,
          CMD_PWCTR2,  1, 0x10,
          CMD_VMCTR1,  2, 0x3e,0x28,
          CMD_VMCTR2,  1, 0x86,
          CMD_FRMCTR1, 2, 0x00,0x13,
          0xF2       , 1, 0x00,

          CMD_GAMMASET,1, 0x01,  // Gamma set, curve 1
//          CMD_GMCTRP1,15, 0x0F,0x31,0x2B,0x0C,0x0E,0x08,0x4E,0xF1,0x37,0x07,0x10,0x03,0x0E,0x09,0x00,
          CMD_GMCTRP1, 15, 0x1F, 0x1A, 0x18, 0x0A, 0x0F, 0x06, 0x45, 0X87, 0x32, 0x0A, 0x07, 0x02, 0x07, 0x05, 0x00,
//          CMD_GMCTRN1, 15, 0x00,0x0E,0x14,0x03,0x11,0x07,0x31,0xC1,0x48,0x08,0x0F,0x0C,0x31,0x36,0x0F,
          CMD_GMCTRN1, 15, 0x00, 0x25, 0x27, 0x05, 0x10, 0x09, 0x3A, 0x78, 0x4D, 0x05, 0x18, 0x0D, 0x38, 0x3A, 0x1F,
          CMD_DFUNCTR, 3, 0x08,0xC2,0x27,
          CMD_SLPOUT , 0+CMD_INIT_DELAY, 120,    // Exit sleep mode
          CMD_IDMOFF , 0,
          CMD_DISPON , 0+CMD_INIT_DELAY, 100,
          0xFF,0xFF, // end
      };
      switch (listno)
      {
      case 0: return list0;
      default: return nullptr;
      }
    }
  };

//----------------------------------------------------------------------------
 }
}


class LGFX : public lgfx::LGFX_Device
{

//lgfx::Panel_GC9A01      _panel_instance;
//lgfx::Panel_GDEW0154M09 _panel_instance;
//lgfx::Panel_HX8357B     _panel_instance;
//lgfx::Panel_HX8357D     _panel_instance;
//lgfx::Panel_ILI9163     _panel_instance;
  lgfx::Panel_ILI9341m     _panel_instance;
//lgfx::Panel_ILI9342     _panel_instance;
//lgfx::Panel_ILI9481     _panel_instance;
//lgfx::Panel_ILI9486     _panel_instance;
//lgfx::Panel_ILI9488     _panel_instance;
//lgfx::Panel_IT8951      _panel_instance;
//lgfx::Panel_RA8875      _panel_instance;
//lgfx::Panel_SH110x      _panel_instance; // SH1106, SH1107
//lgfx::Panel_SSD1306     _panel_instance;
//lgfx::Panel_SSD1327     _panel_instance;
//lgfx::Panel_SSD1331     _panel_instance;
//lgfx::Panel_SSD1351     _panel_instance; // SSD1351, SSD1357
//lgfx::Panel_SSD1963     _panel_instance;
//lgfx::Panel_ST7735      _panel_instance;
//lgfx::Panel_ST7735S     _panel_instance;
//lgfx::Panel_ST7789      _panel_instance;
//lgfx::Panel_ST7796      _panel_instance;


// パネルを接続するバスの種類にあったインスタンスを用意します。
  lgfx::Bus_SPI       _bus_instance;   // SPIバスのインスタンス
//lgfx::Bus_I2C       _bus_instance;   // I2Cバスのインスタンス (ESP32のみ)
//lgfx::Bus_Parallel8 _bus_instance;   // 8ビットパラレルバスのインスタンス (ESP32のみ)

// バックライト制御が可能な場合はインスタンスを用意します。(必要なければ削除)
  lgfx::Light_PWM     _light_instance;

// タッチスクリーンの型にあったインスタンスを用意します。(必要なければ削除)
//lgfx::Touch_CST816S          _touch_instance;
//  lgfx::Touch_FT5x06           _touch_instance; // FT5206, FT5306, FT5406, FT6206, FT6236, FT6336, FT6436
//lgfx::Touch_GSL1680E_800x480 _touch_instance; // GSL_1680E, 1688E, 2681B, 2682B
//lgfx::Touch_GSL1680F_800x480 _touch_instance;
//lgfx::Touch_GSL1680F_480x272 _touch_instance;
//lgfx::Touch_GSLx680_320x320  _touch_instance;
lgfx::Touch_GT911            _touch_instance;
//lgfx::Touch_STMPE610         _touch_instance;
//lgfx::Touch_TT21xxx          _touch_instance; // TT21100
//lgfx::Touch_XPT2046          _touch_instance;

public:

  // コンストラクタを作成し、ここで各種設定を行います。
  // クラス名を変更した場合はコンストラクタも同じ名前を指定してください。
  LGFX(void)
  {
    { // バス制御の設定を行います。
      auto cfg = _bus_instance.config();    // バス設定用の構造体を取得します。

// SPIバスの設定
      cfg.spi_host = VSPI_HOST;     // 使用するSPIを選択  ESP32-S2,C3 : SPI2_HOST or SPI3_HOST / ESP32 : VSPI_HOST or HSPI_HOST
      // ※ ESP-IDFバージョンアップに伴い、VSPI_HOST , HSPI_HOSTの記述は非推奨になるため、エラーが出る場合は代わりにSPI2_HOST , SPI3_HOSTを使用してください。
      cfg.spi_mode = 0;             // SPI通信モードを設定 (0 ~ 3)
      cfg.freq_write = 50000000;    // 送信時のSPIクロック (最大80MHz, 80MHzを整数で割った値に丸められます)
      cfg.freq_read  = 16000000;    // 受信時のSPIクロック
      cfg.spi_3wire  = true;        // 受信をMOSIピンで行う場合はtrueを設定
      cfg.use_lock   = true;        // トランザクションロックを使用する場合はtrueを設定
      cfg.dma_channel = SPI_DMA_CH_AUTO; // 使用するDMAチャンネルを設定 (0=DMA不使用 / 1=1ch / 2=ch / SPI_DMA_CH_AUTO=自動設定)
      // ※ ESP-IDFバージョンアップに伴い、DMAチャンネルはSPI_DMA_CH_AUTO(自動設定)が推奨になりました。1ch,2chの指定は非推奨になります。
      cfg.pin_sclk = 14;            // SPIのSCLKピン番号を設定
      cfg.pin_mosi = 13;            // SPIのMOSIピン番号を設定
      cfg.pin_miso = 12;            // SPIのMISOピン番号を設定 (-1 = disable)
      cfg.pin_dc   = 2;            // SPIのD/Cピン番号を設定  (-1 = disable)
     // SDカードと共通のSPIバスを使う場合、MISOは省略せず必ず設定してください。
//*/
/*
// I2Cバスの設定
      cfg.i2c_port    = 0;          // 使用するI2Cポートを選択 (0 or 1)
      cfg.freq_write  = 400000;     // 送信時のクロック
      cfg.freq_read   = 400000;     // 受信時のクロック
      cfg.pin_sda     = 21;         // SDAを接続しているピン番号
      cfg.pin_scl     = 22;         // SCLを接続しているピン番号
      cfg.i2c_addr    = 0x3C;       // I2Cデバイスのアドレス
//*/
/*
// 8ビットパラレルバスの設定
      cfg.i2s_port = I2S_NUM_0;     // 使用するI2Sポートを選択 (I2S_NUM_0 or I2S_NUM_1) (ESP32のI2S LCDモードを使用します)
      cfg.freq_write = 20000000;    // 送信クロック (最大20MHz, 80MHzを整数で割った値に丸められます)
      cfg.pin_wr =  4;              // WR を接続しているピン番号
      cfg.pin_rd =  2;              // RD を接続しているピン番号
      cfg.pin_rs = 15;              // RS(D/C)を接続しているピン番号
      cfg.pin_d0 = 12;              // D0を接続しているピン番号
      cfg.pin_d1 = 13;              // D1を接続しているピン番号
      cfg.pin_d2 = 26;              // D2を接続しているピン番号
      cfg.pin_d3 = 25;              // D3を接続しているピン番号
      cfg.pin_d4 = 17;              // D4を接続しているピン番号
      cfg.pin_d5 = 16;              // D5を接続しているピン番号
      cfg.pin_d6 = 27;              // D6を接続しているピン番号
      cfg.pin_d7 = 14;              // D7を接続しているピン番号
//*/

      _bus_instance.config(cfg);    // 設定値をバスに反映します。
      _panel_instance.setBus(&_bus_instance);      // バスをパネルにセットします。
    }

    { // 表示パネル制御の設定を行います。
      auto cfg = _panel_instance.config();    // 表示パネル設定用の構造体を取得します。

      cfg.pin_cs           =    15;  // CSが接続されているピン番号   (-1 = disable)
      cfg.pin_rst          =    -1;  // RSTが接続されているピン番号  (-1 = disable)
      cfg.pin_busy         =    -1;  // BUSYが接続されているピン番号 (-1 = disable)

      // ※ 以下の設定値はパネル毎に一般的な初期値が設定されていますので、不明な項目はコメントアウトして試してみてください。

      cfg.panel_width      =   320;  // 実際に表示可能な幅
      cfg.panel_height     =   240;  // 実際に表示可能な高さ
      cfg.offset_x         =     0;  // パネルのX方向オフセット量
      cfg.offset_y         =     80;  // パネルのY方向オフセット量
      cfg.offset_rotation  =    4;  // 回転方向の値のオフセット 0~7 (4~7は上下反転)
      cfg.dummy_read_pixel =     8;  // ピクセル読出し前のダミーリードのビット数
      cfg.dummy_read_bits  =     1;  // ピクセル以外のデータ読出し前のダミーリードのビット数
      cfg.readable         =  false;  // データ読出しが可能な場合 trueに設定
      cfg.invert           = true;  // パネルの明暗が反転してしまう場合 trueに設定
      cfg.rgb_order        = true;  // パネルの赤と青が入れ替わってしまう場合 trueに設定
      cfg.dlen_16bit       = false;  // 16bitパラレルやSPIでデータ長を16bit単位で送信するパネルの場合 trueに設定
      cfg.bus_shared       =  true;  // SDカードとバスを共有している場合 trueに設定(drawJpgFile等でバス制御を行います)

// 以下はST7735やILI9163のようにピクセル数が可変のドライバで表示がずれる場合にのみ設定してください。
//      cfg.memory_width     =   240;  // ドライバICがサポートしている最大の幅
//      cfg.memory_height    =   320;  // ドライバICがサポートしている最大の高さ

      _panel_instance.config(cfg);
    }

//*
    { // バックライト制御の設定を行います。（必要なければ削除）
      auto cfg = _light_instance.config();    // バックライト設定用の構造体を取得します。

      cfg.pin_bl = 27;              // バックライトが接続されているピン番号
      cfg.invert = true;           // バックライトの輝度を反転させる場合 true
      cfg.freq   = 5000;           // バックライトのPWM周波数
      cfg.pwm_channel = 7;          // 使用するPWMのチャンネル番号

      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);  // バックライトをパネルにセットします。
    }
//*/

//*
    { // タッチスクリーン制御の設定を行います。（必要なければ削除）
      auto cfg = _touch_instance.config();

      cfg.x_min      = 0;    // タッチスクリーンから得られる最小のX値(生の値)
      cfg.x_max      = 239;  // タッチスクリーンから得られる最大のX値(生の値)
      cfg.y_min      = 0;    // タッチスクリーンから得られる最小のY値(生の値)
      cfg.y_max      = 319;  // タッチスクリーンから得られる最大のY値(生の値)
      cfg.pin_int    = -1;   // INTが接続されているピン番号
      cfg.bus_shared = false; // 画面と共通のバスを使用している場合 trueを設定
      cfg.offset_rotation = 5;// 表示とタッチの向きのが一致しない場合の調整 0~7の値で設定

// SPI
/*
      cfg.spi_host = VSPI_HOST;// 使用するSPIを選択 (HSPI_HOST or VSPI_HOST)
      cfg.freq = 1000000;     // SPIクロックを設定
      cfg.pin_sclk = 18;     // SCLKが接続されているピン番号
      cfg.pin_mosi = 23;     // MOSIが接続されているピン番号
      cfg.pin_miso = 19;     // MISOが接続されているピン番号
      cfg.pin_cs   =  5;     //   CSが接続されているピン番号
*/
// I2C接続の場合
      cfg.i2c_port = 1;      // 使用するI2Cを選択 (0 or 1)
      cfg.i2c_addr = 0x5D;   // I2Cデバイスアドレス番号
      cfg.pin_sda  = 33;     // SDAが接続されているピン番号
      cfg.pin_scl  = 32;     // SCLが接続されているピン番号
      cfg.freq = 400000;     // I2Cクロックを設定

      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);  // タッチスクリーンをパネルにセットします。
    }
//*/

    setPanel(&_panel_instance); // 使用するパネルをセットします。
  }
};

//#define SCREEN_ROTATION 4