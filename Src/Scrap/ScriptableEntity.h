#pragma once



namespace Project001
{
	class ScriptableEntity
	{
	public:
		ScriptableEntity();
		~ScriptableEntity();

		ScriptableEntity(ScriptableEntity& other) = delete;
		void operator=(const ScriptableEntity&) = delete;

	protected:

	private:
		
	};
}