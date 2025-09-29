/*
OBS Starter Plugin - Configuration Dialog
Copyright (C) 2024

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
*/

#pragma once

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QLabel>
#include <QFileDialog>
#include <QGroupBox>
#include <QResizeEvent>
#include <QCloseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <vector>
#include <string>
#include <plugin-support.h>

// Custom button that draws a cross
class CrossButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CrossButton(QWidget *parent = nullptr) : QPushButton(parent) {}

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QPushButton::paintEvent(event);
        
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        
        // Draw white cross
        painter.setPen(QPen(Qt::white, 2, Qt::SolidLine, Qt::RoundCap));
        
        int margin = 6;
        int w = width() - 2 * margin;
        int h = height() - 2 * margin;
        
        // Draw X shape
        painter.drawLine(margin, margin, margin + w, margin + h);
        painter.drawLine(margin + w, margin, margin, margin + h);
    }
};

class ExecutableSection : public QGroupBox
{
    Q_OBJECT

public:
    ExecutableSection(const ExecutableConfig &config = {}, QWidget *parent = nullptr);
    ExecutableConfig getConfig() const;
    void setConfig(const ExecutableConfig &config);

signals:
    void removeRequested();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void browseForExecutable();
    void onRemoveClicked();

private:
    QLineEdit *pathLineEdit;
    QCheckBox *shutdownCheckBox;
    QCheckBox *minimizeCheckBox;
    QPushButton *browseButton;
    CrossButton *removeButton;
};

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(QWidget *parent = nullptr);
    ~ConfigDialog();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void addSection();
    void removeSection();
    void saveSettings();
    void loadSettings();

private:
    void setupUI();
    
    QVBoxLayout *mainLayout;
    QScrollArea *scrollArea;
    QWidget *scrollWidget;
    QVBoxLayout *scrollLayout;
    QPushButton *addButton;
    QPushButton *saveButton;
    QPushButton *cancelButton;
    
    std::vector<ExecutableSection*> sections;
};