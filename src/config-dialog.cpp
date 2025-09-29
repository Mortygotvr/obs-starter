/*
OBS Starter Plugin - Configuration Dialog Implementation
Copyright (C) 2024

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
*/

#include "config-dialog.h"
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSizePolicy>
#include <QPushButton>
#include <QApplication>
#include <QStyle>
#include <QTimer>

ExecutableSection::ExecutableSection(const ExecutableConfig &config, QWidget *parent)
    : QGroupBox("Executable Configuration", parent)
{
    setFixedHeight(160); // Increased height for additional checkbox
    
    // Create layout
    QGridLayout *layout = new QGridLayout(this);
    
    // Remove button (white cross in upper right)
    removeButton = new CrossButton(this);
    removeButton->setFixedSize(24, 24);
    removeButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #c0c0c0; "
        "    border: none; "
        "    border-radius: 12px; "
        "} "
        "QPushButton:hover { "
        "    background-color: #a0a0a0; "
        "}"
    );
    // Position in top-right corner (will be adjusted after widget is shown)
    connect(removeButton, &QPushButton::clicked, this, &ExecutableSection::onRemoveClicked);
    
    // Path section
    QLabel *pathLabel = new QLabel("Executable Path:", this);
    pathLineEdit = new QLineEdit(this);
    pathLineEdit->setPlaceholderText("Select executable file...");
    pathLineEdit->setMinimumHeight(28); // Taller input field
    
    browseButton = new QPushButton("Browse...", this);
    browseButton->setMinimumHeight(30); // Set to exactly 30 pixels
    browseButton->setMinimumWidth(80);
    connect(browseButton, &QPushButton::clicked, this, &ExecutableSection::browseForExecutable);
    
    // Shutdown checkbox
    shutdownCheckBox = new QCheckBox("Auto-shutdown when OBS closes", this);
    shutdownCheckBox->setChecked(true); // Default enabled
    
    // Minimize checkbox
    minimizeCheckBox = new QCheckBox("Start minimized", this);
    minimizeCheckBox->setChecked(false); // Default disabled
    
    // Layout setup with better spacing and alignment
    layout->addWidget(pathLabel, 0, 0, 1, 1);
    layout->addWidget(pathLineEdit, 0, 1, 1, 2);
    layout->addWidget(browseButton, 0, 3, 1, 1);
    layout->addWidget(shutdownCheckBox, 1, 1, 1, 3);
    layout->addWidget(minimizeCheckBox, 2, 1, 1, 3);
    
    // Adjust row height and alignment to position browse button lower
    layout->setRowMinimumHeight(0, 35);  // Increased from 32 to give more space
    layout->setAlignment(pathLineEdit, Qt::AlignTop);
    layout->setAlignment(browseButton, Qt::AlignCenter);  // Changed to center to lower it
    
    // Remove complex stylesheet positioning, keep it simple
    browseButton->setStyleSheet("QPushButton { padding: 4px 8px; }");
    
    layout->setColumnStretch(1, 1);
    layout->setContentsMargins(30, 30, 15, 15); // More space for the cross button
    layout->setVerticalSpacing(8); // Increased spacing
    layout->setHorizontalSpacing(8);
    
    // Set initial config
    if (!config.path.empty()) {
        setConfig(config);
    }
}

// Override resizeEvent to position the remove button in the top-right corner
void ExecutableSection::resizeEvent(QResizeEvent *event)
{
    QGroupBox::resizeEvent(event);
    // Position remove button in top-right corner with some margin
    removeButton->move(width() - 30, 6);
}

ExecutableConfig ExecutableSection::getConfig() const
{
    ExecutableConfig config;
    config.path = pathLineEdit->text().toStdString();
    config.shutdown_enabled = shutdownCheckBox->isChecked();
    config.start_minimized = minimizeCheckBox->isChecked();
    return config;
}

void ExecutableSection::setConfig(const ExecutableConfig &config)
{
    pathLineEdit->setText(QString::fromStdString(config.path));
    shutdownCheckBox->setChecked(config.shutdown_enabled);
    minimizeCheckBox->setChecked(config.start_minimized);
}

void ExecutableSection::browseForExecutable()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Select Executable",
        "",
        "Executable Files (*.exe);;All Files (*.*)"
    );
    
    if (!fileName.isEmpty()) {
        pathLineEdit->setText(fileName);
    }
}

void ExecutableSection::onRemoveClicked()
{
    emit removeRequested();
}

ConfigDialog::ConfigDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    loadSettings();
    
    setWindowTitle("OBS Starter Configuration");
    setMinimumSize(600, 400);
    resize(700, 500);
    
    // Set window flags to ensure proper cleanup
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
}

ConfigDialog::~ConfigDialog()
{
    // Clean up sections
    for (ExecutableSection *section : sections) {
        if (section) {
            section->deleteLater();
        }
    }
    sections.clear();
}

void ConfigDialog::closeEvent(QCloseEvent *event)
{
    // Accept the close event and hide the dialog
    hide();
    event->accept();
}

void ConfigDialog::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    
    // Create scroll area
    scrollArea = new QScrollArea(this);
    scrollWidget = new QWidget();
    scrollLayout = new QVBoxLayout(scrollWidget);
    
    scrollLayout->setAlignment(Qt::AlignTop);
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    // Add button
    addButton = new QPushButton("Add Executable", this);
    addButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon));
    connect(addButton, &QPushButton::clicked, this, &ConfigDialog::addSection);
    
    // Bottom buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    saveButton = new QPushButton("Save", this);
    cancelButton = new QPushButton("Cancel", this);
    
    saveButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogApplyButton));
    cancelButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCancelButton));
    
    connect(saveButton, &QPushButton::clicked, this, &ConfigDialog::saveSettings);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);
    
    // Main layout
    mainLayout->addWidget(new QLabel("Configure executables to start with OBS:", this));
    mainLayout->addWidget(addButton);
    mainLayout->addWidget(scrollArea);
    mainLayout->addLayout(buttonLayout);
}

void ConfigDialog::addSection()
{
    ExecutableSection *section = new ExecutableSection({}, scrollWidget);
    connect(section, &ExecutableSection::removeRequested, [this, section]() {
        // Remove from layout and vector
        scrollLayout->removeWidget(section);
        auto it = std::find(sections.begin(), sections.end(), section);
        if (it != sections.end()) {
            sections.erase(it);
        }
        section->deleteLater();
    });
    
    scrollLayout->addWidget(section);
    sections.push_back(section);
}

void ConfigDialog::removeSection()
{
    // This is handled by the signal from individual sections
}

void ConfigDialog::saveSettings()
{
    std::vector<ExecutableConfig> configs;
    
    for (ExecutableSection *section : sections) {
        ExecutableConfig config = section->getConfig();
        if (!config.path.empty()) {
            configs.push_back(config);
        }
    }
    
    update_executable_configs(configs);
    
    QMessageBox::information(this, "Settings Saved", 
                           "Configuration has been saved successfully.");
    accept();
}

void ConfigDialog::loadSettings()
{
    // Clear existing sections
    for (ExecutableSection *section : sections) {
        scrollLayout->removeWidget(section);
        section->deleteLater();
    }
    sections.clear();
    
    // Load configurations
    std::vector<ExecutableConfig> configs = get_executable_configs();
    
    for (const ExecutableConfig &config : configs) {
        ExecutableSection *section = new ExecutableSection(config, scrollWidget);
        connect(section, &ExecutableSection::removeRequested, [this, section]() {
            scrollLayout->removeWidget(section);
            auto it = std::find(sections.begin(), sections.end(), section);
            if (it != sections.end()) {
                sections.erase(it);
            }
            section->deleteLater();
        });
        
        scrollLayout->addWidget(section);
        sections.push_back(section);
    }
    
    // Add at least one section if none exist
    if (sections.empty()) {
        addSection();
    }
}