/*
 * Copyright 2016 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "tests/fake_container.h"

#include <QByteArray>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QtCore/QProcess>
#include <QtCore/QString>


class TestAppLauncher : public AppLauncher
{
public:
  TestAppLauncher(const std::string& json)
    :AppLauncher()
  {
    auto doc = QJsonDocument::fromJson(QByteArray::fromStdString(json));
    auto obj = doc.object();
    name_ = obj["name"].toString().toStdString();
    if (obj["icons"].isArray())
    {
      icon_ = obj["icons"].toArray()[0].toString().toStdString();
    }
    description_ = obj["description"].toString().toStdString();
    uri_ = obj["uri"].toString().toStdString();
  }
};

FakeContainer::
FakeContainer(std::string const& json_string)
: Container("fake-container", "fake-container")
{
  QJsonDocument json = QJsonDocument::fromJson(QByteArray::fromStdString(json_string), nullptr);
  QJsonObject object = json.object();
  QJsonValue name = object["name"];
  if (!name.isNull())
  {
    name_ = name.toString().toStdString();

    QJsonValue v = object["app_launchers"];
    if (v != QJsonValue::Undefined)
    {
      for (auto const& app: v.toArray())
      {
        auto json = QJsonDocument(app.toObject()).toJson().toStdString();
        app_launcher_list_.emplace_back(TestAppLauncher(json));
      }
    }
  }
}


FakeContainer::
~FakeContainer()
{ }

