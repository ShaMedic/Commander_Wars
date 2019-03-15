#include "leaf.h"

#include "decisiontree.h"

#include "coreengine/mainapp.h"

Leaf::Leaf(QVector<QVector<float>>& trainingData)
    : m_AnswersChances(DecisionTree::countClassItems(trainingData))
{
    for (qint32 i = 0; i < trainingData.size(); i++)
	{
        float answer = trainingData[i][trainingData[i].size() - 1];
        if (m_Answers.contains(answer) == false)
		{
            m_Answers.append(answer);
		}
	}
    for (qint32 i = 0; i < m_AnswersChances.size(); i++)
    {
        totalChance += m_AnswersChances[i];
    }
}

float Leaf::getDecision(QVector<float>&)
{
    if (m_Answers.size() > 1)
    {
        qint32 chance = Mainapp::randInt(1, totalChance);
        qint32 lowThreshold = 1;
        for (qint32 i = 0; i < m_AnswersChances.size(); i++)
        {
            if ((chance >= lowThreshold) &&
                (chance < lowThreshold + m_AnswersChances[i]))
            {
                return m_Answers[i];
            }
            else
            {
                lowThreshold += m_AnswersChances[i];
            }
        }
    }
	return m_Answers[0];
}

void Leaf::serialize(QDataStream& pStream)
{
    pStream << false; // --> 0 for node and 1 for leaf
    pStream << getVersion();
    pStream << m_AnswersChances.size();
    for (qint32 i = 0; i < m_AnswersChances.size(); i++)
    {
        pStream << m_AnswersChances[i];
    }
    pStream << m_Answers.size();
    for (qint32 i = 0; i < m_Answers.size(); i++)
    {
        pStream << m_Answers[i];
    }
    pStream << totalChance;
}

void Leaf::deserialize(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    qint32 size = 0;
    pStream << size;
    for (qint32 i = 0; i < size; i++)
    {
        qint32 item = 0;
        pStream << item;
        m_AnswersChances.append(item);
    }
    size = 0;
    pStream << size;
    for (qint32 i = 0; i < size; i++)
    {
        qint32 item = 0;
        pStream << item;
        m_Answers.append(item);
    }

    pStream >> totalChance;
}
