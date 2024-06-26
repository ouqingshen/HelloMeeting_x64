#include "CBottomBarTools.h"

CBottomBarTools::CBottomBarTools(QString text, QString normalImageUrl, QString normalImageHoverUrl,
	QString clickedImageUrl, QString clickedImageHoverUrl, bool clicked, QWidget* parent)
	:m_text(text),
	m_normalImageUrl(normalImageUrl),
	m_normalImageHoverUrl(normalImageHoverUrl),
	m_clickedImageUrl(clickedImageUrl),
	m_clickedImageHoverUrl(clickedImageHoverUrl),
	m_StateOpen(clicked),
	QToolButton(parent)
{
	this->setFixedSize(70, 70);
	this->setIconSize(QSize(30, 30));
	this->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	this->setStyleSheet("\
							QToolButton{border:none;font-size:14px;padding-top:3px;padding-bottom:3px}\
							QToolButton:hover{background-color:rgb(204, 204, 204)} \
						");
	this->setText(text);
	this->setIcon(QIcon(m_StateOpen ? m_normalImageUrl : m_clickedImageUrl));
}

CBottomBarTools::~CBottomBarTools()
{
}

bool CBottomBarTools::event(QEvent* e) {
	if (this->isChecked()) { return QWidget::event(e); }

	switch (e->type())
	{
	case QEvent::Enter:
	{
		this->setIcon(QIcon(m_StateOpen ? m_normalImageHoverUrl : m_clickedImageHoverUrl));
		return true;
	}
	case QEvent::MouseButtonPress:
	{
		m_StateOpen = !m_StateOpen;
		this->setIcon(QIcon(m_StateOpen ? m_normalImageUrl : m_clickedImageHoverUrl));
		this->setChecked(true);
		emit sig_clicked();
		return true;
	}
	case QEvent::Leave:
	{
		this->setIcon(QIcon(m_StateOpen ? m_normalImageUrl : m_clickedImageUrl));
		return true;
	}

	default:
		break;
	}

	return QWidget::event(e);
}