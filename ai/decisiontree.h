#ifndef AI_DECISIONTREE
#define AI_DECISIONTREE

#include <QObject>
#include <QVector>

#include "decisionnode.h"
#include "question.h"

#include "coreengine/fileserializable.h"

class DecisionTree : public QObject, public FileSerializable
{
	Q_OBJECT
public:
        DecisionTree(spDecisionNode pRootNode);
        DecisionTree(QVector<QVector<float>>& trainingData, QVector<QVector<spQuestion>>& questions);
        DecisionTree(QString treeFile, QString trainingData);

	virtual ~DecisionTree();

        spDecisionNode train(QVector<QVector<float>>& trainingData, QVector<QVector<spQuestion>>& questions);

	static QVector<qint32> countClassItems(QVector<QVector<float>>& trainingData);

        virtual void serialize(QDataStream& pStream)  override;
        virtual void deserialize(QDataStream& pStream)  override;
        virtual qint32 getVersion() override
        {
            return 1;
        }
public slots:
        float getDecision(QVector<float>& input);
protected:
        void seperateTrueFalse(QVector<QVector<float>>& trainingData, spQuestion question, QVector<QVector<float>>& trueData, QVector<QVector<float>>& falseData);
	float giniImpurity(QVector<QVector<float>>& trainingData);
	float infoGain(QVector<QVector<float>>& trainingDataLeft, QVector<QVector<float>>& trainingDataRight, float currentUncertainty);
        spQuestion findBestSplit(QVector<QVector<float>>& trainingData, float& bestGain, QVector<QVector<spQuestion>>& questions);
private:
        spDecisionNode m_pRootNode;
};

#endif
