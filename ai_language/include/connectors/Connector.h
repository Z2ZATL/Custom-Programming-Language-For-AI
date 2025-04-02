
/**
 * @file Connector.h
 * @brief Base class for external library connectors
 */

#ifndef AI_LANGUAGE_CONNECTOR_H
#define AI_LANGUAGE_CONNECTOR_H

#include "BaseConnector.h"

namespace ai_language {

/**
 * @class Connector
 * @brief Main connector class for interfacing with external libraries
 */
class Connector : public BaseConnector {
public:
    /**
     * @brief Constructor
     */
    Connector() = default;
    
    /**
     * @brief Destructor
     */
    ~Connector() override = default;
    
    /**
     * @brief Connect to external library
     */
    void connect() override;
};

} // namespace ai_language

#endif // AI_LANGUAGE_CONNECTOR_H
