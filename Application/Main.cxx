#include <QApplication>
#include <QDialog>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QStackedLayout>
#include <QStandardPaths>
#include <QVBoxLayout>
#include <QWebEngineCookieStore>
#include <QWebEngineProfile>
#include <QWebEngineView>

int main(int argc, char **argv) {
  QStackedLayout *stackedLayout = new QStackedLayout;
  QApplication app(argc, argv);
  QDialog *weDialog = new QDialog;
  QWebEngineView *weView = new QWebEngineView;
  QWidget *loadingWidget = new QWidget;
  loadingWidget->setLayout(new QVBoxLayout);
  static_cast<QVBoxLayout *>(loadingWidget->layout())
      ->addWidget(new QLabel("Loading F1TV authentication page!"), 0,
                  Qt::AlignCenter);
  QWidget *finishedWidget = new QWidget;
  finishedWidget->setLayout(new QVBoxLayout);
  static_cast<QVBoxLayout *>(finishedWidget->layout())
      ->addWidget(
          new QLabel(
              "F1TV subscription token saved! You can close this application!"),
          0, Qt::AlignCenter);
  stackedLayout->addWidget(loadingWidget);
  stackedLayout->addWidget(weView);
  stackedLayout->addWidget(finishedWidget);
  stackedLayout->setCurrentIndex(0);
  weDialog->setLayout(stackedLayout);
  app.connect(
      weView, &QWebEngineView::loadFinished,
      [stackedLayout, weDialog](bool) { stackedLayout->setCurrentIndex(1); });
  app.connect(QWebEngineProfile::defaultProfile()->cookieStore(),
              &QWebEngineCookieStore::cookieAdded,
              [stackedLayout, weView](const QNetworkCookie &cookie) {
                if (cookie.name() == "login-session" &&
                    cookie.domain().endsWith("formula1.com")) {
                  weView->stop();
                  weView->page()->deleteLater();

                  QDir appDataDir = QDir(QStandardPaths::writableLocation(
                      QStandardPaths::AppDataLocation));

                  if (!appDataDir.exists())
                    appDataDir.mkpath(".");

                  QFile tokenFile(appDataDir.filePath("token.json"));
                  tokenFile.open(QIODevice::WriteOnly);
                  tokenFile.write(
                      QUrl::fromPercentEncoding(cookie.value()).toUtf8());

                  stackedLayout->setCurrentIndex(2);
                }
              });
  weView->load(QUrl("https://account.formula1.com/#/en/login"));
  weDialog->show();
  return app.exec();
}
