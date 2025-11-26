/*****************************************************************//**
 * @file   Object.h
 * @brief  オブジェクトの基底クラスと専用のポインタ
 *
 * ポインタ先の有効性チェックを自動で行う
 *
 * @author 石田怜
 * @date   2025/11/23
 *********************************************************************/
#pragma once

#include <vector>
#include <type_traits>

template<typename ObjectType>
class ObjPtr;

/**
 * @brief コンポーネントを含む全てのオブジェクトの基底クラス
 */
class Object
{
	template<typename ObjectType>
	friend class ObjPtr;

public:
	Object() = default;
	virtual ~Object()
	{
		// 自身を指すポインタを全て無効にする
		InvalidateThisPtr();
	}

	// コピー禁止
	Object(const Object&) = delete;
	Object& operator=(const Object&) = delete;

	virtual void Destroy() = 0;

private:
	/**
	 * @brief 自身を指すポインタを登録する
	 * @param ppObject 登録するポインタへのポインタ
	 */
	void AddThisPtr(Object** ppObject)
	{
		m_thisPtrs.emplace_back(ppObject);
	}

	/**
	 * @brief 自身を指すポインタの登録を削除する
	 * @param ppObject 登録を削除するポインタへのポインタ
	 */
	void RemoveThisPtr(Object** ppObject)
	{
		auto it = std::find(m_thisPtrs.begin(), m_thisPtrs.end(), ppObject);
		if (it != m_thisPtrs.end())
		{
			// 末尾と入れ替えて削除
			*it = m_thisPtrs.back();
			m_thisPtrs.pop_back();
		}
	}

	/**
	 * @brief 自身を指すポインタを全て無効にする
	 */
	void InvalidateThisPtr()
	{
		// 自身を指す全てのポインタにnullptrをセット
		for (auto thisPtr : m_thisPtrs)
		{
			*thisPtr = nullptr;
		}

		m_thisPtrs.clear();
	}

	/// 自身を指すポインタへのポインタを格納するコンテナ
	std::vector<Object**> m_thisPtrs;
};

/**
 * @brief オブジェクト専用のポインタ
 * @details ポインタ先削除時に、自動的に無効化される
 * @details 通常のポインタのように比較演算子や代入演算子、アロー演算子などが使用できる
 */
template <typename ObjectType>
class ObjPtr
{
	static_assert(std::is_base_of<Object, ObjectType>::value,
		"ObjPtrに無効なクラスが指定されました");

public:
	ObjPtr() = default;
	ObjPtr(ObjectType* pObject)
	{
		SetPtr(pObject);
	}
	ObjPtr(const ObjPtr& other)
	{
		m_pObject = nullptr;
		SetPtr(other.m_pObject);
	}
	ObjPtr(ObjPtr&& other)
	{
		m_pObject = nullptr;
		SetPtr(other.m_pObject);
	}
	~ObjPtr()
	{
		Reset();
	}

	// ポインタ先アクセス系演算子
	ObjectType& operator*()
	{
		return *m_pObject;
	}
	ObjectType* operator->()
	{
		return m_pObject;
	}
	const ObjectType& operator*() const
	{
		return *m_pObject;
	}
	const ObjectType* operator->() const
	{
		return m_pObject;
	}

	// 代入演算子
	ObjPtr& operator=(const ObjPtr& ObjPtr)
	{
		SetPtr(ObjPtr.m_pObject);
		return *this;
	}
	ObjPtr& operator=(ObjectType* pObject)
	{
		SetPtr(pObject);
		return *this;
	}
	ObjPtr& operator=(std::nullptr_t)
	{
		Reset();
		return *this;
	}

	// 比較演算子
	bool operator==(const ObjPtr& ObjPtr) const
	{
		return m_pObject == ObjPtr.m_pObject;
	}
	bool operator!=(const ObjPtr& ObjPtr) const
	{
		return m_pObject != ObjPtr.m_pObject;
	}
	bool operator==(std::nullptr_t) const
	{
		return m_pObject == nullptr;
	}
	bool operator!=(std::nullptr_t) const
	{
		return m_pObject != nullptr;
	}
	operator bool() const
	{
		return m_pObject != nullptr;
	}

	/**
	 * @brief ポインタの有効性を確認する
	 * @return ポインタがnullptrならtrueを返す
	 */
	bool IsNull() const
	{
		return m_pObject == nullptr;
	}

	/**
	 * @brief オブジェクトへの実際のポインタを取得する
	 * @return オブジェクトへの生ポインタ
	 */
	ObjectType* Get() const
	{
		return m_pObject;
	}

	/**
	 * @brief ポインタを無効化する
	 */
	void Reset()
	{
		if (m_pObject != nullptr)
		{
			// このポインタの登録を解除
			m_pObject->RemoveThisPtr(reinterpret_cast<Object**>(&m_pObject));
			m_pObject = nullptr;
		}
	}

	/**
	 * @brief ポインタをセットする
	 * @param pObject セットするオブジェクトのポインタ
	 */
	void SetPtr(ObjectType* pObject)
	{
		if (m_pObject == pObject)
			return;

		Reset();
		m_pObject = pObject;

		if (m_pObject != nullptr)
		{
			// このポインタを登録
			m_pObject->AddThisPtr(reinterpret_cast<Object**>(&m_pObject));
		}
	}

private:
	/// オブジェクトへの実際のポインタ
	ObjectType* m_pObject = nullptr;
};