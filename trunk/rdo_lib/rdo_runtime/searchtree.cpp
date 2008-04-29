#include "pch.h"
#include "searchtree.h"
#include "searchtrace.h"
#include "rdo_runtime.h"
#include "rdo_rule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- TreeRoot - ������ ������ DPT
// ----------------------------------------------------------------------------
TreeRoot::TreeRoot( RDOSimulator* sim, RDODPTSearch* _dp ):
	m_dp( _dp ),
	m_rootNode( NULL ),
	m_targetNode( NULL ),
	m_theRealSimulator( sim ),
	m_nodesCount( 0 ),
	m_nodesInGraphCount( 1 ), // ���� ��������� �������
	m_expandedNodesCount( 0 ),
	m_fullNodesCount( 0 ),
	m_sizeof_dpt( 0 )
{
	::GetSystemTime( &m_systime_begin );
}

// ----------------------------------------------------------------------------
// ---------- TreeNode - ���� ����� DPT
// ----------------------------------------------------------------------------
TreeNode::TreeNode( RDOSimulator* _sim, TreeNode* _parent, TreeRoot* _root, RDODPTSearch::Activity* _activity, double cost, int cnt ):
	m_currAct( NULL ),
	m_childSim( NULL ),
	m_newCostPath( 0 ),
	m_newCostRest( 0 ),
	m_newCostRule( 0 ),
	m_sim( _sim ),
	m_parent( _parent ),
	m_root( _root ),
	m_activity( _activity ),
	m_costRule( 0 ),
	m_costPath( cost ),
	m_costRest( 0 ),
	m_number( cnt )
{
	m_sim->onPutToTreeNode();
}

TreeNode::~TreeNode()
{
	for( std::vector<TreeNode*>::iterator i = m_children.begin(); i != m_children.end(); i++ ) {
		delete (*i);
		m_root->m_sizeof_dpt -= sizeof(TreeNode);
	}
	m_root->m_sizeof_dpt -= m_sim->getSizeofSim();
	delete m_sim;
}

int TreeNode::getActivityID() const
{
	return ((RDOActivityTrace*)m_activity)->getTraceID();
}

void TreeNode::ExpandChildren()
{
	m_root->m_sizeof_dpt -= (m_sim->getSizeofSim() + sizeof(TreeNode)) * m_children.size();
	DeleteAllObjects( m_children );

	// ������ ����������
	onSearchOpenNode( m_root->m_theRealSimulator );

	// ��������� �� �������� �������
	// TODO: ��������, ���� ��������� ��� ������� � ������ OPEN
	if ( m_root->m_dp->TermCondition(m_sim) ) {
		m_root->m_targetNode = this;
#ifdef _DEBUG
		static_cast<RDORuntime*>(m_sim)->showResources(m_number);
#endif
		return;
	}

	// ������ ��� ����������
	m_root->m_expandedNodesCount++;

	int s = m_root->m_dp->activities.size();

	// ������ �� ���� ����������� ����� �����
	for ( std::list< RDODPTSearch::Activity* >::iterator i = m_root->m_dp->activities.begin(); i != m_root->m_dp->activities.end(); i++ ) {
		m_currAct  = (*i);
		m_childSim = m_sim->createCopy();
#ifdef _DEBUG
		if ( static_cast<RDORuntime*>(m_childSim)->checkState() ) {
			TRACE( "���������, node = %d\n", m_number );
		}
#endif
		m_root->m_sizeof_dpt += m_childSim->getSizeofSim();
		m_currAct->rule()->onBeforeChoiceFrom( m_childSim );
		if ( !m_currAct->rule()->choiceFrom( static_cast<RDORuntime*>(m_childSim) ) ) {
			// �� ������ Choice from, ������� ��������� � ��������� � ������ ����������.
			// TODO: � ����� ������� ���������, ���� �� �� ���������� ?
			// ����� ����� ��������, ���� ��� ���������� ���������� �������� �����
			// ���������� calc, �� ������� ����� ����������� �� ������ ��� �������
			// �������, ������� ����� �������� ��������� ��������
			// ���� ����� ������� �������������� � ������� ���.
			m_root->m_sizeof_dpt -= m_childSim->getSizeofSim();
			delete m_childSim;
			m_childSim = NULL;
		} else {
			// ������ ��� ����������
			m_root->m_fullNodesCount++;
			// ��������� ��������� ���������� ������� (value before)
			if ( m_currAct->valueTime() == RDODPTSearch::Activity::vt_before ) {
				m_newCostRule = m_currAct->cost( m_childSim );
			}
			// ��������� ���� ������� (�������� �������)
			m_currAct->rule()->onBeforeRule( m_childSim );
			m_currAct->rule()->convertRule( static_cast<RDORuntime*>(m_childSim) );
			m_currAct->rule()->onAfterRule( m_childSim, true );

			// ��������� ��������� ���������� ������� (value after)
			if ( m_currAct->valueTime() == RDODPTSearch::Activity::vt_after ) {
				m_newCostRule = m_currAct->cost( m_childSim );
			}
			// ��������� ��������� ���� �� ������� �������
			m_newCostPath = m_costPath + m_newCostRule;
			// ��������� ��������� ���� �� ���� � ������ ������
			// ������ �� ����� �������� � ������ ��������������� ������ OPEN
			m_newCostRest = m_newCostPath + m_root->m_dp->EvaluateBy( m_childSim );

			// ���� �� ���������� ������� (���������� ��� ����������)
			// �������� ����� ��������
			if ( m_root->m_dp->NeedCompareTops() ) {
				TreeNode* loser = NULL;
				NodeFoundInfo res = m_root->m_rootNode->CheckIfExistBetter( m_childSim, m_newCostPath, &loser );
				if ( res == nfi_found_better ) {
					// � ����� ���� ����� ������ �������, �.�. ������� ����������
					// ���������� ���� (������) �� ������ � ���� �� ���������.
					// ���� ������������� �� ����.
					// ������ ����������� ��������� �������.
					onSearchNodeInfoDeleted( m_root->m_theRealSimulator );
					m_root->m_sizeof_dpt -= m_childSim->getSizeofSim();
					delete m_childSim;
					// ��������� � ��������� ����������
					continue;
				} else if ( res == nfi_found_loser ) {
#ifdef _DEBUG
					if ( m_number == 294 ) {
						TRACE( "loser->m_number = %d\n", loser->m_number );
						TRACE( "loser->m_parent->m_number = %d\n", loser->m_parent->m_number );
						TRACE( "loser->m_parent->m_children.size() = %d\n", loser->m_parent->m_children.size() );
						static_cast<RDORuntime*>(loser->m_sim)->showResources(loser->m_number);
					}
#endif
					// ����� ��������
					// � ����� ������� ������� (loser) � ����� ������ ���������� ����,
					// �.�. ������� ���������� ���������� ����� (�������).
					// ���������� ����������� ����
					// ������� �������� ��� ����� �� ������� �������� loser->parent
					loser->m_parent->m_children.erase( std::find(loser->m_parent->m_children.begin(), loser->m_parent->m_children.end(), loser) );
#ifdef _DEBUG
					if ( m_number == 294 ) {
						TRACE( "loser->m_parent->m_children.size() after erase = %d\n", loser->m_parent->m_children.size() );
					}
#endif
					// ������ ������������� ��������� ���� ������� � ���� � ��������
					// todo: � ��� �� ��� ������, �.�. costPath � ������ �������,
					// ������������ ��� costPath + newCostRule, ����� �� ��������
					// ��-�� newCostRule ��-�� ������ ���������� ���������� ������,
					// �.�. ������ ���� �������� loser->costRule (��. ����), � �����
					// �������� loser->ReCostSubTree( costPath )
					loser->ReCostSubTree( m_costPath );
					// ������ �������� �� ������� �������
					loser->m_parent   = this;
					// ����������� �� ����� �����
					loser->m_number   = m_root->getNewNodeNumber();
					// ��������� ���������
					loser->m_costPath = m_newCostPath;
					loser->m_costRest = m_newCostRest;
					loser->m_costRule = m_newCostRule;
					// ������ ��������� �� ����� ����������
					loser->m_activity = m_currAct;
					// ����, �� ������ ��� ����������� ������ ����������
					loser->m_currAct  = m_currAct;
					// ������� ����������� �� ����� ��������
					loser->onSearchNodeInfoReplaced( m_root->m_theRealSimulator );
					// ��������� � ������ �������� �������
					m_children.push_back( loser );
					m_root->m_sizeof_dpt -= m_childSim->getSizeofSim();
					delete m_childSim;
#ifdef _DEBUG
					if ( m_number == 294 ) {
						static_cast<RDORuntime*>(loser->m_sim)->showResources( loser->m_number );
					}
#endif
					// ��������� � ��������� ����������
					continue;
				}
			}

			// �� ����� � ����� ������ ���������
			// ��� ���� ���������. �������� ��� ����� ����� �������
			TreeNode* tn = createChildTreeNode();
			m_root->m_nodesInGraphCount++;
			tn->m_costPath = m_newCostPath;
			tn->m_costRest = m_newCostRest;
			tn->m_costRule = m_newCostRule;
			tn->m_currAct  = m_currAct;
			// ������� �����������
			tn->onSearchNodeInfoNew( m_root->m_theRealSimulator );
			// ������� � ������ �������� �������
			m_children.push_back(tn);
/*
			if(!root->m_dp->TermCondition(childSim))
				root->OPEN.push_back(tn);
			else
			{
				root->targetNode = tn;
			}
*/
			// ������� � ������ OPEN
			m_root->m_OPEN.push_back( tn );
		}
	}
	// ������ ������� ������� �� ������ OPEN
	m_root->m_OPEN.erase( std::find(m_root->m_OPEN.begin(), m_root->m_OPEN.end(), this) );

	// ����������� ��� ������� � ������ OPEN
	std::sort( m_root->m_OPEN.begin(), m_root->m_OPEN.end(), compareNodes );
}

TreeNode::NodeFoundInfo TreeNode::CheckIfExistBetter( RDOSimulator* childSim, double useCost, TreeNode** loser )
{
	if ( *childSim == *m_sim ) {
		if ( m_costPath <= useCost ) {
			return nfi_found_better;
		} else {
			*loser = this;
			return nfi_found_loser;
		}
	}

	for ( std::vector< TreeNode* >::iterator i = m_children.begin(); i != m_children.end(); i++ ) {
		NodeFoundInfo res = (*i)->CheckIfExistBetter( childSim, useCost, loser );
		if ( res != nfi_notfound ) return res;
	}

	return nfi_notfound;
}

void TreeNode::ReCostSubTree( double cost )
{
	m_costPath = cost + m_costRule;

	for ( std::vector< TreeNode* >::iterator i = m_children.begin(); i != m_children.end(); i++ ) {
		(*i)->ReCostSubTree( m_costPath );
	}
}

TreeNode* TreeNode::createChildTreeNode()
{
	m_root->m_sizeof_dpt += sizeof( TreeNode );
	return new TreeNode(m_childSim, this, m_root, m_currAct, m_costPath, m_root->getNewNodeNumber());
}

} // namespace rdoRuntime;
