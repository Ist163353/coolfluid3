#ifndef CF_GUI_Client_NLog_hpp
#define CF_GUI_Client_NLog_hpp

//////////////////////////////////////////////////////////////////////////////

#include <QObject>
#include <QHash>
#include <QList>

#include "GUI/Client/CNode.hpp"

#include "GUI/Network/LogMessage.hpp"

class QString;
class QIcon;

//////////////////////////////////////////////////////////////////////////////

namespace CF {

namespace GUI {
namespace Client {

/////////////////////////////////////////////////////////////////////////////

  /// @brief Log component
  class NLog :
      public QObject,
      public CNode
  {
    Q_OBJECT

  public:

    typedef boost::shared_ptr<NLog> Ptr;
    typedef boost::shared_ptr<NLog const> ConstPtr;

    /// @brief Constructor.
    NLog();

    /// @brief Destructor.

    /// Frees all allocated memory.
    ~NLog();

    /// @brief Adds a message to the log.

    /// If the message contains '<' or '>' characters, they will replaced
    /// respectively by '&lt;' and '&gt;'.
    /// @param message The message to add.
    void addMessage(const QString & message);

    /// @brief Adds an error message to the log.

    /// If the message contains '<' or '>' characters, they will replaced
    /// respectively by '&lt;' and '&gt;'.
    /// @param message The error message to add.
    void addError(const QString & message);

    /// @brief Adds an exception message to the log.

    /// @param message The exception message to add.
    void addException(const QString & message);

    /// @brief Gives the icon associated to this node
    /// @return Returns the icon associated to this node
    /// @note This method should be reimplemented by all subclasses.
    virtual QIcon getIcon() const;

    /// @brief Gives the text to put on a tool tip
    /// @return The name of the class.
    virtual QString getToolTip() const;

    /// @brief Gives node options.
    /// @param params Reference to a list where options will be put. The
    /// list is cleared before first use.
    virtual void getOptions(QList<NodeOption> & params) const;

  signals:

    /// @brief Signal emitted when a new message arrives.
    /// @param message Message text
    /// @param isError If @c true it is an error message; otherwise it is
    /// a "normal" message.
    void newMessage(const QString & message, bool isError);

    /// @brief Signal emitted when an exception arrives
    /// @param message Exception message
    void newException(const QString & message);

  private:

    /// @brief Hash map that associates a type message to its name in
    /// string format.

    /// The key is the type. The value is the name.
    QHash<CF::GUI::Network::LogMessage::Type, QString> m_typeNames;

    /// @brief Boost slot called when a message comes from the server
    /// @param node Signal node
    void message(CF::Common::XmlNode & node);

    /// @brief Appends a message to the log

    /// If the message contains '<' or '>' characters, they will replaced
    /// respectively by '&lt;' and '&gt;'.
    /// @param type Message type
    /// @param fromServer If @c true, the message comes from the server;
    /// otherwise it comes from the client.
    /// @param message Message
    void appendToLog(CF::GUI::Network::LogMessage::Type type, bool fromServer,
                     const QString & message);

    /// regists all the signals declared in this class
    static void regist_signals ( Component* self ) {}

  }; // class NLog

  ///////////////////////////////////////////////////////////////////////////

} // namespace Client
} // namespace GUI
} // namespace CF

/////////////////////////////////////////////////////////////////////////////

#endif // CF_GUI_Client_NLog_hpp
