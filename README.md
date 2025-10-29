# コーディング規約
## 変数
**変数名はキャメルケースを使用します**
~~~
camelCase
~~~

**グローバル変数・メンバ変数・staticメンバ変数はそれぞれアンダーバーで接頭語を付けます**
~~~
g_global
m_member
s_static
~~~

**ポインタ変数には小文字のpを先頭につけます**
~~~
pEnemy
g_pSaveData
~~~

**クラス名・構造体名・関数名はアッパーキャメルケースを使用します**
~~~
class Player
struct SaveData
void LoadFile()
~~~

**列挙型は型名はアッパーキャメルケース・値は**アッパースネークケースを使用します**
~~~
enum ColorType
{
    RED,
    BLUE,
    GREEN
}
~~~
{}
