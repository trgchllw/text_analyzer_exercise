# Определение ОС
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    # Для macOS
    INSTALL_DIR = /usr/local/bin
    CONFIG_DIR = /usr/local/etc/text_analyzer
    LOG_DIR = /usr/local/var/log/text_analyzer
else
    # Для Linux
    INSTALL_DIR = /usr/bin
    CONFIG_DIR = /etc/text_analyzer
    LOG_DIR = /var/log/text_analyzer
endif

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = text_analyzer
BUILD_DIR = build
SRC_DIR = src

CORE_DIR = $(SRC_DIR)/core
UI_DIR = $(SRC_DIR)/ui
UTILS_DIR = $(SRC_DIR)/utils

CORE_SRC = $(CORE_DIR)/analyzer.c
UI_SRC = $(UI_DIR)/interface.c
UTILS_SRC = $(UTILS_DIR)/logger.c
MAIN_SRC = $(SRC_DIR)/main.c

CORE_OBJ = $(BUILD_DIR)/analyzer.o
UI_OBJ = $(BUILD_DIR)/interface.o
UTILS_OBJ = $(BUILD_DIR)/logger.o
MAIN_OBJ = $(BUILD_DIR)/main.o

compile: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(CORE_OBJ): $(CORE_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(CORE_SRC) -o $(CORE_OBJ)

$(UI_OBJ): $(UI_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(UI_SRC) -o $(UI_OBJ)

$(UTILS_OBJ): $(UTILS_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(UTILS_SRC) -o $(UTILS_OBJ)

$(MAIN_OBJ): $(MAIN_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(MAIN_SRC) -o $(MAIN_OBJ)

$(TARGET): $(CORE_OBJ) $(UI_OBJ) $(UTILS_OBJ) $(MAIN_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

user_man:
	@echo "Для генерации user_man.pdf: pdflatex docs/user_man.tex"

in_instruct:
	@echo "Для генерации install_instruct.pdf: pdflatex docs/install_instruct.tex"

an_instruct:
	@echo "Для генерации analyze_instruct.pdf: pdflatex docs/analyze_instruct.tex"

license:
	@echo "Для генерации LICENSE.pdf: pdflatex LICENSE.tex"

docs: user_man in_instruct an_instruct license

configure: scripts/configure
	@echo "Запуск конфигурации..."
	@chmod +x scripts/configure
	@./scripts/configure

install: compile configure
	@echo "=== УСТАНОВКА Text Analyzer ==="
	@mkdir -p $(LOG_DIR)
	@chmod 755 $(LOG_DIR)
	@echo "Директория логов: $(LOG_DIR)"
	@mkdir -p $(CONFIG_DIR)
	@cp install.conf $(CONFIG_DIR)/
	@if [ "$(UNAME_S)" != "Darwin" ]; then \
		sudo chown root $(CONFIG_DIR)/install.conf; \
		sudo chmod 644 $(CONFIG_DIR)/install.conf; \
		echo "Сертификат установлен (root:644)"; \
	else \
		echo "Сертификат установлен (macOS)"; \
	fi
	@cp $(TARGET) $(INSTALL_DIR)/
	@chmod 755 $(INSTALL_DIR)/$(TARGET)
	@ln -sf $(INSTALL_DIR)/$(TARGET) /usr/local/bin/analyzer 2>/dev/null || ln -sf $(INSTALL_DIR)/$(TARGET) /usr/bin/analyzer
	@echo "Программа установлена в $(INSTALL_DIR)"
	@echo "=== Установка завершена ==="

all: install

clear_tmp:
	@rm -rf $(BUILD_DIR) *.log *.aux *.toc *.out *.pdfsync

clear_config:
	@rm -f install.conf
	@rm -rf $(CONFIG_DIR)

clear_compile: clear_tmp
	@rm -f $(TARGET)

clear: clear_tmp clear_config

uninstall:
	@echo "=== ДЕИНСТАЛЛЯЦИЯ ==="
	@rm -f $(INSTALL_DIR)/$(TARGET)
	@rm -f /usr/local/bin/analyzer /usr/bin/analyzer 2>/dev/null
	@rm -rf $(CONFIG_DIR)
	@echo "Программа и конфигурация удалены"
	@echo "Логи сохранены в $(LOG_DIR)"
	@echo "=== Деинсталляция завершена ==="

remove: uninstall
	@echo "Удаление без сохранения данных..."

remove_all: remove
	@rm -rf $(LOG_DIR)
	@echo "Логи удалены"

.PHONY: compile user_man in_instruct an_instruct license docs configure install all clear_tmp clear_compile clear clear_config uninstall remove remove_all

clean: clear