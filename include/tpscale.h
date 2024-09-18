#ifndef TPSCALE_H
#define TPSCALE_H

#include <QWidget>
#include <QList>
#include <QPainter>
#include <QtUiPlugin/QDesignerExportWidget>

class QDESIGNER_WIDGET_EXPORT TPScale : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(double level READ level WRITE setLevel NOTIFY levelChanged)
    Q_PROPERTY(double warnLevelLow READ warnLevelLow WRITE setWarnLevelLow)
    Q_PROPERTY(double warnLevelHigh READ warnLevelHigh WRITE setWarnLevelHigh)
    Q_PROPERTY(double protLevelLow READ protLevelLow WRITE setProtLevelLow)
    Q_PROPERTY(double protLevelHigh READ protLevelHigh WRITE setProtLevelHigh)
    Q_PROPERTY(double highest READ highest WRITE setHighest)
    Q_PROPERTY(double lowest READ lowest WRITE setLowest)
    Q_PROPERTY(int division READ division WRITE setDivision)
    Q_PROPERTY(double targetLevel READ targetLevel WRITE setTargetLevel)

public:
    explicit TPScale(QWidget *parent = nullptr);

    double level() const;
    void setLevel(double level);

    double warnLevelLow() const;
    void setWarnLevelLow(double warnLevelLow);

    double warnLevelHigh() const;
    void setWarnLevelHigh(double warnLevelHigh);

    double protLevelLow() const;
    void setProtLevelLow(double protLevelLow);

    double protLevelHigh() const;
    void setProtLevelHigh(double protLevelHigh);

    double highest() const;
    void setHighest(double highest);

    double lowest() const;
    void setLowest(double lowest);

    int division() const;
    void setDivision(int division);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    double targetLevel() const;
    void setTargetLevel(double targetLevel);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

signals:
    void levelChanged(double level);

private:
    void checkWarnLevel();
    void updateLabels();

    double m_level = 0;
    double m_warnLevelLow = 0.0;
    double m_warnLevelHigh = 600.0;
    double m_protLevelLow = 0.0;
    double m_protLevelHigh = 600.0;
    double m_highest = 600.0;
    double m_lowest = 0.0;
    int m_division = 5;
    double m_targetLevel = 0.0;

    QList<QString> m_labels;
};


#endif // TPSCALE_H
