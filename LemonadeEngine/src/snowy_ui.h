#pragma once

#ifndef LEMONADE_ENGINE_SRC_SNOWY_UI_H
#define LEMONADE_ENGINE_SRC_SNOWY_UI_H

#include <memory>
#include <vector>
#include <string>
#include <iostream>

namespace ui
{
	using UINode_ID = const char*;

	class UINode : public std::enable_shared_from_this<UINode>
	{
	private:
		UINode_ID m_ID;

		std::weak_ptr<UINode> m_parent;
		std::vector<std::shared_ptr<UINode>> m_children;

		bool m_queued_for_deletion = false;

	public:

		constexpr UINode( const UINode_ID& ID ):
			m_ID( ID )
		{}

		virtual ~UINode()
		{
			IF_DEBUG( std::cout << "UINode \"" << m_ID << "\" freed." << std::endl;)
		}

		// -- Getters -- //
		constexpr UINode_ID get_id() const { return m_ID; }
		constexpr bool has_id( const UINode_ID& ID ) const { return m_ID == ID; }
		constexpr bool is_queued_for_deletion() const { return m_queued_for_deletion; }

		constexpr bool has_children() const
		{
			if ( m_children.empty() )
				return false;

			// Check if any aren't null
			for ( const auto& child : m_children )
				if ( child ) return true;

			return false;
		}

		constexpr bool has_child( UINode* child ) const
		{
			if ( !child ) return false;
			if ( !has_children() ) return false;

			for ( const auto& other_child : m_children )
				if ( other_child.get() == child ) return true;

			return false;
		}

		constexpr bool has_child( const UINode_ID& child_ID ) const
		{
			if ( !has_children() ) return false;

			for ( const auto& other_child : m_children )
				if ( other_child->has_id( child_ID ) ) return true;

			return false;
		}

		bool has_parent() const { return !m_parent.expired(); }
		bool is_parent( const UINode_ID& parent_ID ) const
		{
			if ( !has_parent() )
				return false;

			return m_parent.lock()->has_id( parent_ID );
		}
		constexpr bool is_parent( UINode* parent ) const
		{
			if ( !parent )
				return false;

			if ( !has_parent() )
				return false;

			return m_parent.lock().get() == parent;
		}

		// -- Indexing -- //

		constexpr size_t find_child( UINode* child )
		{
			for ( size_t i = 0; i < m_children.size(); i++ )
				if ( m_children[i].get() == child ) return i;

			return m_children.size();
		}

		constexpr size_t find_child( const UINode_ID& child_ID )
		{
			for ( size_t i = 0; i < m_children.size(); i++ )
				if ( m_children[i]->has_id( child_ID ) ) return i;

			return m_children.size();
		}

		std::shared_ptr<UINode> get_child( const UINode_ID& child_ID )
		{
			for ( size_t i = 0; i < m_children.size(); i++ )
				if ( m_children[i]->has_id( child_ID ) ) return m_children[i];

			return nullptr;
		}

		// -- Setters -- //

		std::weak_ptr<UINode> set_parent( const std::weak_ptr<UINode>& parent )
		{
			m_parent = parent;
			return m_parent;
		}

		std::weak_ptr<UINode> set_parent( UINode* parent )
		{
			if ( !parent )
				m_parent.reset();
			else
				m_parent = std::shared_ptr<UINode>( parent );

			return m_parent;
		}

		std::shared_ptr<UINode> add_child( const std::shared_ptr<UINode>& child )
		{
			if ( !child ) return nullptr;
			if ( has_child( child.get() ) ) return child;

			m_children.push_back( child );
			child->set_parent( weak_from_this() );
			return child;
		}

		std::shared_ptr<UINode> add_child( UINode* child )
		{
			return add_child( std::shared_ptr<UINode>( child ) );
		}

		constexpr void remove_child( UINode* child )
		{
			if ( !child || !has_child( child ) ) return;

			if ( !child->is_queued_for_deletion() )
			{
				child->queue_free();
				return;
			}

			if ( child->is_parent( this ) )
				child->set_parent( nullptr );

			m_children.erase( m_children.begin() + find_child( child ) );
		}

		void remove_child( const UINode_ID& child_ID )
		{
			if ( !has_child( child_ID ) ) return;
			auto child = get_child( child_ID );
			if ( !child ) return;

			if ( !child->is_queued_for_deletion() )
			{
				child->queue_free();
				return;
			}

			if ( child->is_parent( this ) )
				child->set_parent( nullptr );

			m_children.erase( m_children.begin() + find_child( child_ID ) );
		}

		// -- Misc -- //

		void queue_free_child( UINode* child )
		{
			if ( !child || !has_child( child ) ) return;
			child->queue_free();
		}

		void queue_free_child( const UINode_ID& child_ID )
		{
			if ( !has_child( child_ID ) ) return;
			auto child = get_child( child_ID );
			if ( child ) child->queue_free();
		}

		void queue_free()
		{
			if ( m_queued_for_deletion ) return;
			m_queued_for_deletion = true;

			if ( has_children() )
			{
				for ( const auto& child : m_children )
					if ( child ) child->queue_free();
			}

			if ( !has_parent() ) return;

			if ( auto p = m_parent.lock() )
			{
				p->remove_child( this );
			}
		}

		void print( size_t depth = 0 )
		{
			for ( size_t i = 0; i < depth; i++ )
				std::cout << "    ";

			std::cout << m_ID << std::endl;
			if ( has_children() )
			{
				for ( const auto& child : m_children )
					if ( child ) child->print( depth + 1 );
			}
		}

	public:
		// -- Loop -- //

		// Update

		void update_children()
		{
			if ( !has_children() ) return;

			for ( const auto& child : m_children )
				if ( child ) child->update();
		}

		virtual void update() { update_children(); }

// Render

		void render_children()
		{
			if ( !has_children() ) return;

			for ( const auto& child : m_children )
				if ( child ) child->render();
		}

		virtual void render() { render_children(); }
	};
}

#endif // !LEMONADE_ENGINE_SRC_SNOWY_UI_H