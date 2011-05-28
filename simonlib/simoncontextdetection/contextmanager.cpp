#include "contextmanager.h"
#include "KDE/KService"
#include "KDebug"
#include <QTextStream>

ContextManager* ContextManager::m_instance;

//for testing
#include <QFile>

ContextManager::ContextManager(QObject *parent) :
    QObject(parent)
{

}

void ContextManager::test()
{
  //for testing
   QFile testXml;
    testXml.setFileName("/home/anash/Documents/QtProjects/ProcessListing-build-desktop/test.xml");
    testXml.open(QFile::ReadWrite);
    QDomDocument doc;
    doc.setContent(&testXml);
    testXml.close();

    QDomElement elem;
    elem = doc.documentElement();

    getCompoundCondition(elem);
    //end for testing
}

ContextManager::~ContextManager()
{
    foreach(Condition* condition, m_conditions)
    {
        delete condition;
    }
    foreach(CompoundCondition* cCondition, m_compoundConditions)
    {
        delete cCondition;
    }
}

ContextManager* ContextManager::instance()
{
    if (!m_instance)
        m_instance = new ContextManager();

    return m_instance;
}

Condition* ContextManager::getCondition(const QDomElement &elem)
{
    Condition* condition;
    QString source;
    QString str;
    QTextStream stream(&str);
    
    //check to see if the condition has already been created
    //if so, just return the existing condition
    elem.save(stream, 4);
    kDebug() << "Condition: " + str;
    condition = m_conditionLookup.value(str, NULL);
    if (condition != NULL)
    {
	kDebug() << "Condition is a duplicate!";
	return condition;
    }

    //get the name of the service
    source = elem.attribute("name");

    //get the service
    KService::Ptr service = KService::serviceByStorageId(source);
    if (!service) {
      kDebug() << "Service not found! Source: " << source;
      condition=0;
      return condition;
    }

    //create the factory for the service
    KPluginFactory *factory = KPluginLoader(service->library()).factory();
    if (factory) {
      condition = factory->create<Condition>();
      factory->deleteLater();
    }
    else {
      kDebug() << "Factory not found! Source: " << source;
      condition=0;
      return condition;
    }

    //deserialize the service data
    condition->deSerialize(elem);
    
    //add the condition to member containers for future lookup
    m_conditions.push_back(condition);
    m_conditionLookup.insert(str, condition);

    return condition;
}


CompoundCondition* ContextManager::getCompoundCondition(const QDomElement &elem)
{
    QDomElement conditionElem;
    QList<Condition*> conditions;
    Condition* condition;
    int i=0;

    conditionElem = elem.firstChildElement("Condition");
    while(!conditionElem.isNull())
    {
	condition = getCondition(conditionElem);
	
	if (condition != NULL)
	{
	  conditions.push_back(condition);
	}
	
	kDebug() << "Condition: " << i;
	i++;
	
	conditionElem = conditionElem.nextSiblingElement("Condition");
    }

    m_compoundConditions.push_back(new CompoundCondition(conditions));
    return m_compoundConditions.back();
}

