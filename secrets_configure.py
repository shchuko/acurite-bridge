import os
from configparser import ConfigParser

Import('projenv')


path = os.path.abspath('config/secrets.ini')

if os.path.exists(path):
    print(f"secrets.ini found at {path}, parsing")

    config = ConfigParser()
    config.read('config/secrets.ini')

    if config.get("secret", "loki_logging_enabled") == 'true':
        print("Loki logging is enabled, configuring")

        loki_url = config.get("secret", "loki_url")
        loki_username = config.get("secret", "loki_username")
        loki_password = config.get("secret", "loki_password")
        projenv.Append(CPPDEFINES=[
            ("LOKI_LOGGING_ENABLED", "true"),
            ("LOKI_URL", projenv.StringifyMacro(loki_url)),
            ("LOKI_USERNAME", projenv.StringifyMacro(loki_username)),
            ("LOKI_PASSWORD", projenv.StringifyMacro(loki_password)),
        ])

        print(f"Loki logging CPPDEFINES are set")

    else:
        print("Loki logging is disabled")
