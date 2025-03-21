# DistLedger

Distributed Systems Project 2022/2023

## Authors

**Group A55**

### Code Identification

In all source files (namely in the *groupId*s of the POMs), replace __GXX__ with your group identifier. The group
identifier consists of either A or T followed by the group number - always two digits. This change is important for 
code dependency management, to ensure your code runs using the correct components and not someone else's.

### Team Members

| Number | Name             | User                                                  | Email                                          |
|--------|------------------|-------------------------------------------------------|------------------------------------------------|
| 89532  | Rafael Rodrigues | [Rafael-R](https://github.com/Rafael-R)               | <mailto:rafael.s.rodrigues@tecnico.ulisboa.pt> |
| 97048  | Nuno Lúcio       | [nuno-lucio](https://github.com/nuno-lucio)           | <mailto:nuno.lucio@tecnico.ulisboa.pt>         |
| 99215  | Fábio Santos     | [FabioSourceCode](https://github.com/FabioSourceCode) | <mailto:fabio.alexandre@tecnico.ulisboa.pt>    |

## Getting Started

The overall system is made up of several modules. The main server is the _DistLedgerServer_. The clients are the _User_ 
and the _Admin_. The definition of messages and services is in the _Contract_. The future naming server
is the _NamingServer_.

See the [Project Statement](https://github.com/tecnico-distsys/DistLedger) for a complete domain and system description.

### Prerequisites

The Project is configured with Java 17 (which is only compatible with Maven >= 3.8), but if you want to use Java 11 you
can too -- just downgrade the version in the POMs.

To confirm that you have them installed and which versions they are, run in the terminal:

```s
javac -version
mvn -version
```

### Installation

To compile and install all modules (root directory):

```s
mvn clean install
```

### Running

Naming Server:
```s
cd NamingServer/
mvn exec:java
```

Server (Primary):
```s
cd DistLedgerServer/
mvn exec:java -Dexec.args="2001 A"
```

Server (Secondary):
```s
cd DistLedgerServer/
mvn exec:java -Dexec.args="2002 B"
```

User:
```s
cd User/
mvn exec:java
```

Admin:
```s
cd Admin/
mvn exec:java
```

Debug mode supported on all applications by running command with "-Ddebug" flag.
Example:
```s
mvn exec:java -Dexec.args="2001 A" -Ddebug
```

## Built With

* [Maven](https://maven.apache.org/) - Build and dependency management tool;
* [gRPC](https://grpc.io/) - RPC framework.
