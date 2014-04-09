////////////////////////////////////////////////////////////////////////////////
/// @brief transaction context
///
/// @file
///
/// DISCLAIMER
///
/// Copyright 2004-2013 triAGENS GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is triAGENS GmbH, Cologne, Germany
///
/// @author Jan Steemann
/// @author Copyright 2011-2013, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef TRIAGENS_TRANSACTION_CONTEXT_H
#define TRIAGENS_TRANSACTION_CONTEXT_H 1

#include "Basics/Common.h"
#include "Utils/CollectionNameResolver.h"
#include "VocBase/vocbase.h"

namespace triagens {
  namespace transaction {

    class Collection;

    class Manager;

    class Transaction;

    class WorkUnit;

// -----------------------------------------------------------------------------
// --SECTION--                                                     class Context
// -----------------------------------------------------------------------------

    class Context {

////////////////////////////////////////////////////////////////////////////////
/// @brief Context
////////////////////////////////////////////////////////////////////////////////

      private:
        Context (Context const&);
        Context& operator= (Context const&);

// -----------------------------------------------------------------------------
// --SECTION--                                      constructors and destructors
// -----------------------------------------------------------------------------

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief create a transaction context
////////////////////////////////////////////////////////////////////////////////

        Context (Manager*,
                 struct TRI_vocbase_s*,
                 Context**); 

////////////////////////////////////////////////////////////////////////////////
/// @brief destroy a transaction context
////////////////////////////////////////////////////////////////////////////////

        ~Context ();

// -----------------------------------------------------------------------------
// --SECTION--                                                    public methods
// -----------------------------------------------------------------------------

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief return the next work unit id inside the context
////////////////////////////////////////////////////////////////////////////////
  
        inline uint64_t nextWorkUnitId () {
          return ++_nextWorkUnitId;
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief return the current transaction
////////////////////////////////////////////////////////////////////////////////

        inline Transaction* transaction () const {
          return _transaction;
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief return the current nesting level
////////////////////////////////////////////////////////////////////////////////

        inline int level () const {
          return _workUnits.size();
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief resolve a collection name
////////////////////////////////////////////////////////////////////////////////

        TRI_vocbase_col_t* resolveCollection (std::string const&) const;

////////////////////////////////////////////////////////////////////////////////
/// @brief find a collection in the top-level work units
////////////////////////////////////////////////////////////////////////////////

        Collection* findCollection (TRI_voc_cid_t) const;

////////////////////////////////////////////////////////////////////////////////
/// @brief get or create a transaction context
////////////////////////////////////////////////////////////////////////////////

        static Context* getContext (Manager*, 
                                    struct TRI_vocbase_s*,
                                    Context**);

////////////////////////////////////////////////////////////////////////////////
/// @brief create a transaction context
////////////////////////////////////////////////////////////////////////////////

        static Context* getContext (Manager*,
                                    struct TRI_vocbase_s*);

////////////////////////////////////////////////////////////////////////////////
/// @brief increase the reference count
////////////////////////////////////////////////////////////////////////////////

        void increaseRefCount (); 

////////////////////////////////////////////////////////////////////////////////
/// @brief decrease the reference count
/// the last user that calls this will destroy the context!
////////////////////////////////////////////////////////////////////////////////

        void decreaseRefCount (); 

////////////////////////////////////////////////////////////////////////////////
/// @brief start a new unit of work
////////////////////////////////////////////////////////////////////////////////

        int startWorkUnit (WorkUnit*);

////////////////////////////////////////////////////////////////////////////////
/// @brief end a unit of work
////////////////////////////////////////////////////////////////////////////////

        int endWorkUnit (WorkUnit*);

// -----------------------------------------------------------------------------
// --SECTION--                                                   private methods
// -----------------------------------------------------------------------------

      private:

////////////////////////////////////////////////////////////////////////////////
/// @brief create a new transaction context
////////////////////////////////////////////////////////////////////////////////

        static Context* createContext (Manager*,
                                       struct TRI_vocbase_s*,
                                       Context**);

// -----------------------------------------------------------------------------
// --SECTION--                                                 private variables
// -----------------------------------------------------------------------------

      private:

////////////////////////////////////////////////////////////////////////////////
/// @brief the transaction manager
////////////////////////////////////////////////////////////////////////////////

        Manager* _manager;

////////////////////////////////////////////////////////////////////////////////
/// @brief the collection name resolver
////////////////////////////////////////////////////////////////////////////////
        
        triagens::arango::CollectionNameResolver _resolver;

////////////////////////////////////////////////////////////////////////////////
/// @brief address to update with our context
////////////////////////////////////////////////////////////////////////////////

        Context** _globalContext;

////////////////////////////////////////////////////////////////////////////////
/// @brief the underlying transaction
////////////////////////////////////////////////////////////////////////////////

        Transaction* _transaction;

////////////////////////////////////////////////////////////////////////////////
/// @brief the current work units
////////////////////////////////////////////////////////////////////////////////

        std::vector<WorkUnit*> _workUnits;

////////////////////////////////////////////////////////////////////////////////
/// @brief id for next work unit
////////////////////////////////////////////////////////////////////////////////

        uint64_t _nextWorkUnitId;

////////////////////////////////////////////////////////////////////////////////
/// @brief the current number of users of this context
////////////////////////////////////////////////////////////////////////////////
        
        int _refCount;

    };

  }
}

#endif

// Local Variables:
// mode: outline-minor
// outline-regexp: "/// @brief\\|/// {@inheritDoc}\\|/// @addtogroup\\|/// @page\\|// --SECTION--\\|/// @\\}"
// End:
