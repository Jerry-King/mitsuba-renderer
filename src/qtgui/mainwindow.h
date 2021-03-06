/*
    This file is part of Mitsuba, a physically based rendering system.

    Copyright (c) 2007-2011 by Wenzel Jakob and others.

    Mitsuba is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    Mitsuba is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "common.h"
#include <QtNetwork>
#include <mitsuba/render/renderjob.h>

#define MAX_RECENT_FILES 10

namespace Ui {
    class MainWindow;
}

class QConsoleAppender;
class ServerWidget;
class GLWidget;
class PreviewSettingsDlg;

class Ui_WiscombeBRDFSettings;
class Ui_HanrahanKruegerBRDFSettings;
class Ui_DipoleBSSRDFSettings;
class Ui_MultipoleBSSRDFSettings;
class Ui_AdipoleBSSRDFSettings;
class Ui_ShahRealtimeSettings;
/**
 * Captures progress notifications from mitsuba and helps to transfer
 * them to the Qt event loop.
 */
class QRenderListener : public QObject, public RenderListener {
	Q_OBJECT
public:
	/// Called when work has begun in a rectangular image region
	inline void workBeginEvent(const RenderJob *job, const RectangularWorkUnit *wu, int worker) {
		emit workBegin(job, wu, worker);
	}

	/// Called when work has finished in a rectangular image region
	inline void workEndEvent(const RenderJob *job, const ImageBlock *wr) {
		emit workEnd(job, wr);
	}

	/// Called when the whole target image has been altered in some way
	inline void refreshEvent(const RenderJob *job, const Bitmap *bitmap) {
		emit refresh(job, bitmap);
	}

	/// Called when a render job has completed successfully or unsuccessfully
	inline void finishJobEvent(const RenderJob *job, bool cancelled) {
		emit jobFinished(job, cancelled);
	}

	MTS_DECLARE_CLASS()
signals:
	void workBegin(const RenderJob *job, const RectangularWorkUnit *wu, int worker);
	void workEnd(const RenderJob *job, const ImageBlock *wr);
	void refresh(const RenderJob *job, const Bitmap *bitmap);
	void jobFinished(const RenderJob *job, bool cancelled);
protected:
	virtual ~QRenderListener() { }
};

/**
 * A small event eater class for QT.
 */
class EventEater : public QObject {
    Q_OBJECT

protected:
    bool eventFilter(QObject *obj, QEvent *event) {
        return true;
    }
};

class PreviewSettingsDialog;
class LogWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    typedef std::vector<Shape *> shapeListType;
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
	void loadFile(QString filename);
	void adjustSize();
	bool isActive();
	void initWorkers();

protected:
	SceneContext *loadScene(const QString &filename);
	void resizeEvent(QResizeEvent *event);
    void changeEvent(QEvent *e);
	void updateRecentFileActions();
	void addRecentFile(QString fileName);
	void closeEvent(QCloseEvent *event);
	SceneContext *getContext(const RenderJob *job, bool failIfNotFound = true);
	void drawHLine(SceneContext *ctx, int x1, int y, int x2, const float *color);
	void drawVLine(SceneContext *ctx, int x, int y1, int y2, const float *color);
	void drawVisualWorkUnit(SceneContext *context, const VisualWorkUnit &block);
	void checkForUpdates(bool notifyIfNone = false);
	void saveAs(SceneContext *ctx, const QString &targetFile);
	QSize sizeHint() const;
    void resetPreview(SceneContext* context);
    void changeSnowCoefficient(int wlIndex, double value);
    void updateSnowOnAllShapes(SceneContext *context, bool visible = true);
    void updateSnowOnShape(SceneContext* context, Shape* shape, bool createNew, bool updateUi = true);
    void blockRenderComponentsSignals(bool block);

signals:
	void updateView();

private slots:
	void on_actionRenderSettings_triggered();
	void on_actionPreviewSettings_triggered();
	void on_actionOpen_triggered();
	void on_actionExit_triggered();
	void on_actionAbout_triggered();
	void on_actionRefresh_triggered();
	void on_actionRender_triggered();
	void on_actionClose_triggered();
	void on_actionStop_triggered();
	void on_actionShowLog_triggered();
	void on_actionSettings_triggered();
	void on_actionUpdateCheck_triggered();
    void on_actionStartServer_triggered();
    void on_actionAllShapesSnow_triggered();
    void on_actionAllShapesNoSnow_triggered();
	void on_actionSave_triggered();
	void on_actionSaveAs_triggered();
	void on_actionExportImage_triggered();
	void on_actionExportShape_triggered();
	void on_actionNavigationControls_triggered();
	void on_actionReferenceManual_triggered();
	void on_actionImport_triggered();
	void on_actionDuplicateTab_triggered();
	void on_actionAdjustSize_triggered();
	void on_actionReportBug_triggered();
	void on_actionFeedback_triggered();
	void on_actionShowKDTree_triggered();
	void on_actionSceneDescription_triggered();
	void on_actionShowNormals_triggered();
	void on_actionSerialJobsRendering_triggered();
	void on_actionSerialSubjobsRendering_triggered();
	void on_tabBar_currentChanged(int index);
	bool on_tabBar_tabCloseRequested(int index);
	void on_tabBar_tabMoved(int from, int to);
	void on_tabBar_customContextMenuRequested(const QPoint &pt);
	void on_glView_loadFileRequest(const QString &string);
	void onOpenRecent();
	void onClearRecent();
	void onWorkBegin(const RenderJob *job, const RectangularWorkUnit *wu, int worker);
	void onWorkEnd(const RenderJob *job, const ImageBlock *wr);
	void onRefresh(const RenderJob *job, const Bitmap *bitmap);
	void onJobFinished(const RenderJob *job, bool cancelled);
	void onProgressMessage(const RenderJob *job, const QString &name, 
		float progress, const QString &eta);
	void onStatusMessage(const QString &status);
	void onNetworkFinished(QNetworkReply *reply);
	void onServerClosed();
	void onBugReportError();
	void onBugReportSubmitted();
	void updateUI();
	void updateSnowComponents();
    void updateShapeComponents();
    void updateSnowRenderingComponents();
	void updateStatus();
	void onPreviewSettingsClose();
	void onOpenDialogClose(int reason);
	void onSaveAsDialogClose(int reason);
	void onExportDialogClose(int reason);
	void onExportShapeDialogClose(int reason);
	void onRenderSettingsClose(int reason);
	void onImportDialogClose(int reason);
	void onSceneInformationClose(int reason);
	void setNormalScaling(Float scaling);

    void onSnowTypeChanged(int index);
    void onGrainSizeChanged(double size);
    void onDensityChanged(double density);
    void onIorChanged(double ior);
    void onAsymmetryFactorChanged(double g);
    void onCalculationTypeChanged(int index);
    void onSsAlbedoChanged(double ssAlbedo);
    void on435nmCoeffChanged(double coeff);
    void on545nmCoeffChanged(double coeff);
    void on700nmCoeffChanged(double coeff);
    void onSelectedShapeChanged(int shape);
    void onToggleSnowMaterial(int state);
    void onSnowRenderModelChange();
    void onDipoleTextureSettingsChanged();
    void onDipoleZrTextureLoad();
    void onDipoleZrTextureOpenDialogClose(int reason);
    void onDipoleSigmaTrTextureLoad();
    void onDipoleSigmaTrTextureOpenDialogClose(int reason);
    void onDipoleIrrtrrDumpPathRequest();
    void onDipoleIrrtrrDumpPathDialogClose(int reason);
    void onRefreshShahSnowParameters();
    void onShahAlbedoMapPathRequest();
    void onShahAlbedoMapPathDialogClose(int reason);
    void loadShahCustomAlbedoMap();

private:
    Ui::MainWindow *ui;
	QAction *m_actRecent[MAX_RECENT_FILES];
	QAction *m_clearRecent;
	QList<SceneContext *> m_context;
	ref<RenderQueue> m_renderQueue;
	ref<QRenderListener> m_renderListener;
	Thread::EThreadPriority m_workerPriority;
	QList<ServerConnection> m_connections;
	QMutex m_contextMutex;
	PreviewSettingsDialog *m_previewSettingsDialog;
	LogWidget *m_logWidget;
	ServerWidget *m_serverWidget;
	ref<QConsoleAppender> m_consoleAppender;
	QNetworkAccessManager *m_networkManager;
	QNetworkReply *m_networkReply;
	QProgressBar *m_progress;
	QLabel *m_progressLabel;
	QWidget *m_progressWidget;
	QString m_statusMessage;
	QStringList m_searchPaths;
	QString m_nodeName;
	int m_blockSize, m_listenPort;
	bool m_checkForUpdates, m_manualUpdateCheck;
	bool m_activeWindowHack;
	int m_bugStatus, m_contextIndex;
	SceneContext *m_lastTab;
#if defined(__OSX__)
	PreviewSettingsDlg *m_previewSettings;
#endif
	QWidget *m_currentChild;

    QGroupBox *m_wiscombeWidget;
    Ui_WiscombeBRDFSettings *m_wiscombeSettings;

    QGroupBox *m_hkWidget;
    Ui_HanrahanKruegerBRDFSettings *m_hkSettings;

    QGroupBox *m_dipoleWidget;
    Ui_DipoleBSSRDFSettings *m_dipoleSettings;

    QGroupBox *m_multipoleWidget;
    Ui_MultipoleBSSRDFSettings *m_multipoleSettings;

    QGroupBox *m_adipoleWidget;
    Ui_AdipoleBSSRDFSettings *m_adipoleSettings;

    QGroupBox *m_shahRTWidget;
    Ui_ShahRealtimeSettings *m_shahRTSettings;
};

#endif // MAINWINDOW_H
