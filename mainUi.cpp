#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <clocale>
#include "database/metrodatabase.h"
#include "model/station.h"
#include "algorithm/pathfinder.h"



bool isRussianUpper(const std::string& str, size_t pos) {
  if (pos + 1 >= str.size()) return false;

  const unsigned char c1 = str[pos];
  const unsigned char c2 = str[pos+1];

  return (c1 == 0xD0 && c2 >= 0x90 && c2 <= 0x9F) ||
         (c1 == 0xD0 && c2 >= 0xA0 && c2 <= 0xAF) ||
         (c1 == 0xD1 && c2 >= 0x80 && c2 <= 0x8F);
}

void convertRussianUpperToLower(std::string& result, const std::string& str, size_t& pos) {
  if (pos + 1 >= str.size()) return;

  const unsigned char c1 = str[pos];
  const unsigned char c2 = str[pos+1];

  if (c1 == 0xD0 && c2 >= 0x90 && c2 <= 0x9F) {
      result += c1;
      result += static_cast<char>(c2 + 0x20);
      pos += 2;
  }
  else if (c1 == 0xD0 && c2 >= 0xA0 && c2 <= 0xAF) {
      result += 0xD1;
      result += static_cast<char>(c2 - 0x20);
      pos += 2;
  }
  else if (c1 == 0xD0 && c2 == 0x81) {
      result += 0xD1;
      result += 0x91;
      pos += 2;
  }
  else {
      result += str[pos++];
  }
}

std::string toLower(const std::string& str) {
  std::string result;
  result.reserve(str.size());

  for (size_t i = 0; i < str.size(); ) {
      unsigned char c = str[i];

      if ((c & 0x80) == 0) {
          if (c >= 'A' && c <= 'Z') {
              result += static_cast<char>(c + 32);
          } else {
              result += c;
          }
          i++;
      }
      else if (isRussianUpper(str, i)) {
          convertRussianUpperToLower(result, str, i);
      }
      else {
          size_t char_len = 1;
          if ((c & 0xE0) == 0xC0) char_len = 2;
          else if ((c & 0xF0) == 0xE0) char_len = 3;
          else if ((c & 0xF8) == 0xF0) char_len = 4;

          for (size_t j = 0; j < char_len && i < str.size(); j++) {
              result += str[i++];
          }
      }
  }

  return result;
}
// Параметры интерфейса
const float margin = 50.f;
const float leftPanelWidth = 450.f; // ширина левой панели
const float buttonWidth = 250.f;
const float inputHeight = 40.f;
const float spacing = 70.f;
const float startY = 140.f;
const float mapWidth = 800.f;
const float mapHeight = 600.f;
const float mapMarginRight = margin; // отступ справа
const float mapMarginTop = margin;   // отступ сверху

const float maxInputWidth = 450.f;
const float minInputWidth = 250.f;
float mapScale = 0.6f; // масштаб по умолчанию (600/1000 примерно)
sf::Vector2f mapOffset = {50.f, 0.f}; // смещение карты внутри области

// Функция добавления Unicode символа в UTF-8 строку
void addUnicodeToString(std::string &str, sf::Uint32 unicode) {
  if (unicode < 128) {
    str += static_cast<char>(unicode);
  } else if (unicode < 2048) {
    str += static_cast<char>((unicode >> 6) + 192);
    str += static_cast<char>((unicode & 63) + 128);
  } else if (unicode < 65536) {
    str += static_cast<char>((unicode >> 12) + 224);
    str += static_cast<char>(((unicode >> 6) & 63) + 128);
    str += static_cast<char>((unicode & 63) + 128);
  } else {
    str += static_cast<char>((unicode >> 18) + 240);
    str += static_cast<char>(((unicode >> 12) & 63) + 128);
    str += static_cast<char>(((unicode >> 6) & 63) + 128);
    str += static_cast<char>((unicode & 63) + 128);
  }
}

// Функция удаления последнего UTF-8 символа из строки
void removeLastUTF8Char(std::string &str) {
  if (str.empty())
    return;
  int pos = str.length() - 1;
  while (pos > 0 && (str[pos] & 0xC0) == 0x80) {
    pos--;
  }
  str.erase(pos);
}
std::string formatRouteToString(const std::vector<std::string>& path) {
  std::stringstream ss;

  if (path.empty()) {
      return "Маршрут не найден";
  }

  ss << "Кратчайший маршрут (" << path.size()-1 << " переходов): ";

  for (size_t i = 0; i < path.size(); ++i) {
      ss << path[i];
      if (i != path.size()-1) {
          ss << " -> ";
      }
  }

  return ss.str();
}


// Функция для обертки текста по ширине с помощью SFML шрифта
std::string wrapTextToFitWidth(const std::string &text, const sf::Font &font,
                               unsigned int characterSize, float maxWidth) {
  std::stringstream ss(text);
  std::string word, line, result;
  sf::Text tempText;
  tempText.setFont(font);
  tempText.setCharacterSize(characterSize);

  while (ss >> word) {
    std::string testLine = line + (line.empty() ? "" : " ") + word;
    tempText.setString(sf::String::fromUtf8(testLine.begin(), testLine.end()));
    if (tempText.getLocalBounds().width > maxWidth) {
      result += line + "\n";
      line = word;
    } else {
      line = testLine;
    }
  }
  if (!line.empty())
    result += line;

  return result;
}

// Получение общей высоты многострочного текста с межстрочным отступом
float getTextHeight(const std::string &text, const sf::Font &font,
                    unsigned int characterSize, float maxWidth) {
  sf::Text tempText;
  tempText.setFont(font);
  tempText.setCharacterSize(characterSize);

  float totalHeight = 0.f;
  std::string line;
  std::istringstream stream(text);
  while (std::getline(stream, line)) {
    tempText.setString(line);
    totalHeight +=
        tempText.getLocalBounds().height + 5.f; // 5 - межстрочный отступ
  }
  return totalHeight;
}

int main() {
  MetroDatabase metroDB;
  if (!metroDB.open("metro.db")) {
      std::cerr << "Не удалось открыть базу данных" << std::endl;
      return 1;
  }

  auto stations = metroDB.loadStations();
  metroDB.loadConnections(stations);



  // if (path.empty()) {
  //     std::cout << "Путь не найден!" << std::endl;
  // } else {
  //     std::cout << "Кратчайший маршрут (" << path.size()-1 << " переходов): ";
  //     for (size_t i = 0; i < path.size(); ++i) {
  //         std::cout << path[i];
  //         if (i != path.size()-1) {
  //             std::cout << " -> ";
  //         }
  //     }
  //     std::cout << std::endl;
  // }
  sf::RenderWindow window(sf::VideoMode(1350, 800), "Moscow metro");
  window.setFramerateLimit(60);

  sf::Font font;
  if (!font.loadFromFile("/home/mikestarikov/MetroTest/DejaVuSans.ttf")) {
    std::cerr << "Не удалось загрузить шрифт\n";
    return -1;
  }

  sf::Color bgColor(255, 255, 255);

  sf::Texture metroTexture;
  sf::Sprite metroSprite;
  bool metroMapLoaded =
      metroTexture.loadFromFile("/home/mikestarikov/MetroTest/metro_map.PNG");
  if (metroMapLoaded) {
    metroSprite.setTexture(metroTexture);
    // Убираем жесткое масштабирование, масштабируем динамически ниже
    metroSprite.setPosition(50.f, 0.f);
  }

  // Область карты — вся высота окна, ширина — всё, что справа от левой панели
  sf::FloatRect mapArea(400, // x - слева
                        mapMarginTop,                  // y - сверху
                        mapWidth,                      // ширина
                        mapHeight                      // высота
  );

  bool dragging = false;
  sf::Vector2f dragStartMapOffset;

  // Заголовок
  sf::Text titleText;
  titleText.setFont(font);
  titleText.setCharacterSize(40);
  titleText.setFillColor(sf::Color::Black);
  std::string title = "Московское метро";
  titleText.setString(sf::String::fromUtf8(title.begin(), title.end()));
  titleText.setPosition(25, margin);

  // Вводные строки
  int inputActive = 0; // 0 - ничего, 1 - inputA, 2 - inputB

  // Поля ввода
  sf::RectangleShape inputABox(sf::Vector2f(100.f, inputHeight));
  inputABox.setPosition(margin, startY);

  sf::RectangleShape inputBBox(sf::Vector2f(100.f, inputHeight));
  inputBBox.setPosition(margin, startY + spacing);

  // Кнопка построения маршрута
  sf::RectangleShape findButton(sf::Vector2f(buttonWidth, 50.f));
  findButton.setPosition(margin, startY + 2 * spacing);
  findButton.setFillColor(sf::Color(227, 26, 33));
  findButton.setOutlineColor(sf::Color::Black);
  findButton.setOutlineThickness(2.f);

  sf::Text findButtonText;
  findButtonText.setFont(font);
  findButtonText.setCharacterSize(22);
  findButtonText.setFillColor(sf::Color::White);
  std::string button = "Построить маршрут";
  findButtonText.setString(sf::String::fromUtf8(button.begin(), button.end()));
  sf::FloatRect btnBounds = findButtonText.getLocalBounds();
  findButtonText.setOrigin(btnBounds.left + btnBounds.width / 2.f,
                           btnBounds.top + btnBounds.height / 2.f);
  findButtonText.setPosition(margin + buttonWidth / 2.f,
                             startY + 2 * spacing + 25.f);

  // Метки
  sf::Text labelA;
  labelA.setFont(font);
  std::string station1 = "Станция A:";
  labelA.setString(sf::String::fromUtf8(station1.begin(), station1.end()));
  labelA.setCharacterSize(20);
  labelA.setFillColor(sf::Color::Black);
  labelA.setPosition(margin, startY - 30.f);

  sf::Text labelB;
  labelB.setFont(font);
  std::string station2 = "Станция B:";
  labelB.setString(sf::String::fromUtf8(station2.begin(), station2.end()));
  labelB.setCharacterSize(20);
  labelB.setFillColor(sf::Color::Black);
  labelB.setPosition(margin, startY + spacing - 30.f);

  // Результирующий блок
  const float resultBoxHeight =
      window.getSize().y - (startY + 3 * spacing) - margin;
  sf::RectangleShape routeResultBox(sf::Vector2f(400, resultBoxHeight));
  routeResultBox.setPosition(margin, startY + 3 * spacing);
  routeResultBox.setFillColor(sf::Color::White);
  routeResultBox.setOutlineColor(sf::Color(150, 150, 150));
  routeResultBox.setOutlineThickness(1.f);

  sf::Text routeResultText;
  routeResultText.setFont(font);
  routeResultText.setCharacterSize(20);
  routeResultText.setFillColor(sf::Color::Black);
  routeResultText.setPosition(routeResultBox.getPosition().x + 5.f,
                              routeResultBox.getPosition().y + 10.f);

  // Тексты внутри полей ввода
  sf::Text inputAText, inputBText;
  inputAText.setFont(font);
  inputBText.setFont(font);
  inputAText.setCharacterSize(24);
  inputBText.setCharacterSize(24);
  inputAText.setFillColor(sf::Color::Black);
  inputBText.setFillColor(sf::Color::Black);

  sf::Vector2f dragStartMousePos;
  std::string stationA, stationB;

  // Главный цикл окна
  while (window.isOpen()) {
    sf::Event event;
  std::string lowerStart = toLower(stationA);
  std::string lowerEnd = toLower(stationB);

  int startId = -1, endId = -1;
  for (const auto& station : stations) {
      std::string stationLower = toLower(station.getName());
      if (stationLower == lowerStart) startId = station.getId();
      if (stationLower == lowerEnd) endId = station.getId();
  }

  auto path = PathFinder::findShortestPath(startId, endId, stations);

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
      // Обработка мыши для карты — drag и zoom, только если мышь в области
      // карты
      sf::Vector2f mousePosF =
          window.mapPixelToCoords(sf::Mouse::getPosition(window));
      bool mouseInMapArea = mapArea.contains(mousePosF);

      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left && mouseInMapArea) {
          dragging = true;
          dragStartMousePos = mousePosF;
          dragStartMapOffset = mapOffset;
        }
      }
      if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          dragging = false;
        }
      }
      if (event.type == sf::Event::MouseMoved) {
        if (dragging) {
          sf::Vector2f diff = mousePosF - dragStartMousePos;
          mapOffset = dragStartMapOffset + diff;
          float scaledWidth = metroTexture.getSize().x * mapScale;
          float scaledHeight = metroTexture.getSize().y * mapScale;

          mapOffset.x =
              std::min(0.f, std::max(mapOffset.x, mapArea.width - scaledWidth));
          mapOffset.y = std::min(
              0.f, std::max(mapOffset.y, mapArea.height - scaledHeight));
        }
      }

      // Zoom колёсиком, если в области карты
      if (event.type == sf::Event::MouseWheelScrolled && mouseInMapArea) {
        float delta = event.mouseWheelScroll.delta;
        float oldScale = mapScale;

        if (event.mouseWheelScroll.delta > 0) {
          mapScale *= 1.1f;
        } else {
          mapScale /= 1.1f;
        }
        // ограничение масштаба
        if (mapScale < 0.4f)
          mapScale = 0.4f;
        if (mapScale > 2.0f)
          mapScale = 2.0f;

        // Чтобы зум был "к мыши", корректируем offset:
        sf::Vector2f beforeZoom = (mousePosF - mapOffset) / oldScale;
        sf::Vector2f afterZoom = beforeZoom * mapScale;
        mapOffset += (mousePosF - mapOffset) - afterZoom;
        float scaledWidth = metroTexture.getSize().x * mapScale;
        float scaledHeight = metroTexture.getSize().y * mapScale;
        // Ограничиваем смещение после зума
        mapOffset.x =
            std::min(0.f, std::max(mapOffset.x, mapArea.width - scaledWidth));
        mapOffset.y =
            std::min(0.f, std::max(mapOffset.y, mapArea.height - scaledHeight));
      }
      // Обработка кликов мыши
      if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          dragging = false;
        }
        sf::Vector2f mousePos =
            window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (inputABox.getGlobalBounds().contains(mousePos))
          inputActive = 1;
        else if (inputBBox.getGlobalBounds().contains(mousePos))
          inputActive = 2;
        else if (findButton.getGlobalBounds().contains(mousePos)) {
          // Нажали кнопку построения маршрута — выводим текст маршрута

          std::string result =
              "Маршрут от " + (stationA.empty() ? "[не указано]" : stationA) +
              " до " + (stationB.empty() ? "[не указано]" : stationB) + ":" + formatRouteToString(path) + " ";

          // Определяем ширину самого длинного слова для динамической ширины
          // окна
          std::istringstream iss(result);
          std::string word;
          float maxWordWidth = 0.f;

          sf::Text tempText;
          tempText.setFont(font);
          tempText.setCharacterSize(20);

          while (iss >> word) {
            tempText.setString(sf::String::fromUtf8(word.begin(), word.end()));
            float w = tempText.getLocalBounds().width;
            if (w > maxWordWidth)
              maxWordWidth = w;
          }

          float padding = 20.f;
          float desiredWidth = std::max(buttonWidth, maxWordWidth + padding);
          float maxAllowedWidth = window.getSize().x - 2 * margin;
          desiredWidth = std::min(desiredWidth, maxAllowedWidth);

          routeResultBox.setSize(
              sf::Vector2f(desiredWidth, routeResultBox.getSize().y));

          // Оборачиваем текст с учётом новой ширины
          std::string wrappedResult =
              wrapTextToFitWidth(result, font, 20, desiredWidth - 10.f);
          routeResultText.setString(
              sf::String::fromUtf8(wrappedResult.begin(), wrappedResult.end()));

          // Обновляем высоту бокса под текст
          float neededHeight =
              getTextHeight(wrappedResult, font, 20, desiredWidth);
          float maxAvailableHeight =
              window.getSize().y - routeResultBox.getPosition().y - margin;

          routeResultBox.setSize(sf::Vector2f(
              desiredWidth, std::min(neededHeight + 20.f, maxAvailableHeight)));

          inputActive = 0;
        } else {
          inputActive = 0;
        }

        if (event.type == sf::Event::MouseMoved && dragging) {
          sf::Vector2f mousePos =
              window.mapPixelToCoords(sf::Mouse::getPosition(window));
          sf::Vector2f diff = mousePos - dragStartMousePos;
          mapOffset = dragStartMapOffset + diff;
        }
      }

      // Обработка ввода текста
      if (event.type == sf::Event::TextEntered &&
          (inputActive == 1 || inputActive == 2)) {
        std::string &target = (inputActive == 1) ? stationA : stationB;
        sf::Text *targetText = (inputActive == 1) ? &inputAText : &inputBText;

        if (event.text.unicode == 8) { // Backspace
          removeLastUTF8Char(target);
        } else if (event.text.unicode >= 32) { // Печатаемые символы
          std::string temp = target;
          addUnicodeToString(temp, event.text.unicode);

          targetText->setString(sf::String::fromUtf8(temp.begin(), temp.end()));
          float textWidth = targetText->getLocalBounds().width + 20.f;

          if (textWidth <= maxInputWidth) {
            addUnicodeToString(target, event.text.unicode);
          }
        }

      }
    }

    // Обновляем текст в полях ввода
    inputAText.setString(
        sf::String::fromUtf8(stationA.begin(), stationA.end()));
    inputBText.setString(
        sf::String::fromUtf8(stationB.begin(), stationB.end()));

    // Подгоняем размеры полей ввода под длину текста
    float widthA = std::max(
        minInputWidth,
        std::min(maxInputWidth, inputAText.getLocalBounds().width + 20.f));
    float widthB = std::max(
        minInputWidth,
        std::min(maxInputWidth, inputBText.getLocalBounds().width + 20.f));

    inputABox.setSize(sf::Vector2f(widthA, inputHeight));
    inputBBox.setSize(sf::Vector2f(widthB, inputHeight));

    // Окрас рамок активного поля
    inputABox.setOutlineColor(inputActive == 1 ? sf::Color::Blue
                                               : sf::Color::Black);
    inputABox.setOutlineThickness(inputActive == 1 ? 3.f : 2.f);

    inputBBox.setOutlineColor(inputActive == 2 ? sf::Color::Blue
                                               : sf::Color::Black);
    inputBBox.setOutlineThickness(inputActive == 2 ? 3.f : 2.f);

    // Позиционирование
    inputABox.setPosition(margin, startY);
    inputBBox.setPosition(margin, startY + spacing);

    inputAText.setPosition(inputABox.getPosition().x + 5.f,
                           inputABox.getPosition().y + 5.f);
    inputBText.setPosition(inputBBox.getPosition().x + 5.f,
                           inputBBox.getPosition().y + 5.f);
    window.clear(bgColor);

    // Рисуем карту метро
    if (metroMapLoaded) {
      metroSprite.setPosition(mapOffset);
      metroSprite.setScale(mapScale, mapScale);

      sf::View mapView(sf::FloatRect(0.f, 0.f, mapArea.width, mapArea.height));
      mapView.setViewport(
          sf::FloatRect((leftPanelWidth + margin) / window.getSize().x, 0.f,
                        mapArea.width / window.getSize().x, 1.f));
      window.setView(mapView);
      window.draw(metroSprite);

      // Возвращаем стандартный вид, чтобы рисовать UI на весь экран
      window.setView(window.getDefaultView());
    }

    // Отрисовка интерфейса
    window.draw(titleText);

    window.draw(labelA);
    window.draw(inputABox);
    window.draw(inputAText);

    window.draw(labelB);
    window.draw(inputBBox);
    window.draw(inputBText);

    window.draw(findButton);
    window.draw(findButtonText);

    window.draw(routeResultBox);
    window.draw(routeResultText);

    window.display();
  }


  return 0;
}
