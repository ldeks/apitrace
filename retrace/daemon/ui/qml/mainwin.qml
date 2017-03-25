import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import ApiTrace 1.0
import Qt.labs.settings 1.0

Selection {
    id: selection
}

FrameRetrace {
    id : frameRetrace
    selection: selection
    argvZero: Qt.application.arguments[0]
    onFrameCountChanged: {
        if (frameCount < progressBar.targetFrame) {
            progressBar.frameCount = frameCount;
            return;
        }
        progressBar.visible = false;
        mainUI.visible = true;
    }
    onGeneralErrorChanged: {
        fileError.text = frameRetrace.generalError;
        fileError.detailedText = frameRetrace.generalErrorDetails;
        fileError.visible = true;
    }
}

MessageDialog {
    id: fileError
    title: "FrameRetrace Error"
    icon: StandardIcon.Warning
    visible: false
    onAccepted: {
        visible = false;
        if (frameRetrace.errorSeverity == FrameRetrace.Fatal) {
            Qt.quit();
        }
    }
}

ColumnLayout {
    id: mainUI
    anchors.fill: parent
    visible: false

    RefreshControl {
        Layout.minimumHeight: 20
        Layout.maximumHeight: 20
        metricsModel: frameRetrace
        Layout.alignment: Qt.AlignTop
        Layout.fillWidth: true
        Layout.fillHeight: true
    }
    
    BarGraphControl {
        selection: selection
        metric_model: frameRetrace
        Layout.preferredHeight: 50
        Layout.alignment: Qt.AlignTop
        Layout.fillWidth: true
        Layout.fillHeight: true
    }

    TabView {
        Layout.preferredWidth: 400
        Layout.alignment: Qt.AlignTop
        Layout.fillWidth: true
        Layout.fillHeight: true
        Tab {
            title: "Shaders"
            anchors.fill: parent
            RenderShadersControl {
                anchors.fill: parent
                renderModel: frameRetrace.shaders
            }
        }
        Tab {
            title: "RenderTarget"
            Row {
                Column {
                    id: renderOptions
                    CheckBox {
                        text: "Clear before render"
                        onCheckedChanged: {
                            frameRetrace.clearBeforeRender = checked;
                        }
                    }
                    CheckBox {
                        text: "Stop at render"
                        onCheckedChanged: {
                            frameRetrace.stopAtRender = checked;
                        }
                    }
                    CheckBox {
                        text: "Highlight selected render"
                        onCheckedChanged: {
                            frameRetrace.highlightRender = checked;
                        }
                    }
                }
                Item {
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    width: parent.width - renderOptions.width
                    Image {
                        id: rtDisplayImage
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                        source: frameRetrace.renderTargetImage
                        cache: false
                    }
                }
            }
        }
        Tab {
            title: "Api Calls"
            clip: true
            anchors.fill: parent
            ApiControl {
                anchors.fill: parent
                apiModel: frameRetrace.api
            }
        }
        Tab {
            title: "Metrics"
            id: metricTab
            anchors.fill: parent
            clip: true
            MetricTabControl {
                metricsModel: frameRetrace.metricTab
            }
        }
    }
}
