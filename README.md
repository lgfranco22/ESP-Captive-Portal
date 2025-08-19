# 🔌 Firmware ESP8266 – Captive Portal com Registro

> ⚠️ **Aviso Importante:**  
> Este projeto é apenas **um modelo demonstrativo** para fins de estudo, PoC e aprendizado.  
> **Não deve ser utilizado para fins maliciosos, ilegais ou que possam causar danos a terceiros.**  
> O autor **não se responsabiliza** por qualquer uso inadequado do código ou do firmware aqui disponibilizado.

## 📖 Funcionalidades
- Criação de rede Wi-Fi própria (`SSID: NetFull`).
- Redirecionamento automático para página de login.
- Registro de logins em `logins.txt` (armazenado no **LittleFS**).
- Página `/pwnd` para visualizar registros.
- Página `/clear` para limpar registros.
- LED integrado pisca quando um novo dispositivo se conecta.

## 🚀 Como usar
### 1. Compilar e enviar via Arduino IDE
- Instale o [ESP8266 Core](https://github.com/esp8266/Arduino).
- Carregue o código no ESP8266 (NodeMCU, Wemos D1 Mini, etc).

### 2. Usar firmware pronto
- Baixe o arquivo `.bin` na aba [Releases](../../releases).
- Grave no ESP8266 usando `esptool.py`:

```bash
esptool.py --port COM3 write_flash 0x0 firmware-v1.0.0.bin
```
*(substitua `COM3` pela porta correta do seu dispositivo)*

### 3. Upload via navegador
 - Baixe o arquivo `.bin` na aba [Releases](../../releases).
 - Acesse o site [ESPWebTool](https://esptool.spacehuhn.com/).
 - Conecte a placa e faça o upload do firmware diretamente pelo navegador.

## 🛠️ Requisitos
* Placa: ESP8266 (NodeMCU, Wemos D1 Mini, etc).
* Arduino IDE ou PlatformIO.
* LittleFS para armazenamento de páginas HTML.

## 📜 Licença

Este projeto está sob a licença **MIT** – veja o arquivo [LICENSE](LICENSE) para mais detalhes.
