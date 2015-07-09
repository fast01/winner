#ifndef H_PROJECT_APPLICATION_H__
#define H_PROJECT_APPLICATION_H__
namespace project{
	/** Application **/
	class Application: public core::ApplicationBase{
		SUPPORT_NEWABLE
		typedef core::ApplicationBase Super;
	private:
		Application();
		virtual ~Application();
	public:
		virtual void init();
		virtual void finalize();
	protected:
		virtual bool on_load(const int argc, const char** argv);
		virtual void on_unload();
	protected:
		virtual void register_misc(); 
		virtual void register_lua(); 
		virtual void register_protocol(); 
		virtual void register_class(); 
	};
}

#endif
