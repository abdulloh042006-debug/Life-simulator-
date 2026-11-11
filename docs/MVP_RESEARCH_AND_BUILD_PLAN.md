# Hayot Simulyatori — MVP Research & Build Plan

**Tadqiqot sanasi:** 11-noyabr 2026  
**Loyiha:** https://github.com/abdulloh042006-debug/Life-simulator-  
**Maqsadli platforma:** Windows PC, single-player  
**Tavsiya etilgan engine:** Unreal Engine 5.8 stable  
**Holat:** Manbalar va arxitektura tadqiqoti. Quyidagi pluginlar va assetlar hali asosiy loyihada import/compile/integration testdan o'tkazilmagan.

## 1. MVP maqsadlari

MVP talablarini kichraytirmaymiz:

| Qism | Maqsad |
|---|---|
| Xarita | Bitta kichik, to'liq ishlaydigan mahalla |
| O'yinchi | 1 ta uchinchi shaxs personaji |
| NPC | 10–15 ta faol, kundalik jadvalga ega NPC |
| Binolar | 5–8 ta, 3–5 tasi ichiga kiriladigan |
| Do'konlar | 2–3 ta, zaxira va savdo bilan |
| Transport | Dastlab piyoda; keyin 1 ta transport |
| Kasblar | 2–3 ta, vazifa, maosh va tajriba bilan |
| Iqtisodiyot | Ish haqi, xarid, sotuv, zaxira, talab-taklif, dinamik narx |
| Inventar | Buyum olish/saqlash/ishlatish, stack, vazn yoki sig'im |
| Vaqt/dunyo | Kun-tun, ob-havo, NPC jadvali |
| Saqlash | Player, inventar, pul, ish, vaqt, do'kon va muhim dunyo holati |
| Ovoz | Qadamlar, muhit, eshik/interaction, UI, keyinchalik NPC ovozlari |

**Yondashuv:** tayyor ochiq kodli modullar va qonuniy assetlarni topib, bittadan tekshirish. O'yinga xos Job, Economy, NPC Schedule, World Time va save qoidalari yagona o'z arxitekturamizda qoladi. Litsenziyasiz kod ko'chirilmaydi.

## 2. Engine va loyiha bazasi

- UE 5.8 stable tavsiya qilinadi.
- UE 5.9 Preview asosiy ishlab chiqarish versiyasi bo'lmasin; stable va plugin mosligi tekshirilgach ko'rib chiqiladi.
- Boshlang'ich loyiha: Unreal Third Person Template.
- City Sample — asosiy loyiha emas, shahar/crowd/transport texnologiyalarini o'rganish va kerakli extracted assetlarni alohida baholash uchun. Epic tavsiya qilgan talablari og'ir: 64 GB RAM va RTX 2080/Radeon 6000 sinfidagi GPU.

Rasmiy manbalar:
- UE 5.8: https://www.unrealengine.com/news/unreal-engine-5-8-is-now-available
- UE 5.9 release notes: https://dev.epicgames.com/documentation/unreal-engine/unreal-engine-5-9-release-notes
- Third Person Template: https://dev.epicgames.com/documentation/unreal-engine/third-person-template-in-unreal-engine
- Epic sample projects: https://www.unrealengine.com/unreal-engine-5
- City Sample: https://dev.epicgames.com/documentation/unreal-engine/city-sample-project-unreal-engine-demonstration
- UE hardware requirements: https://dev.epicgames.com/documentation/en-us/unreal-engine/hardware-and-software-specifications-for-unreal-engine

## 3. GitHub kodlari va pluginlar

### 3.1 InteractionSystem-UE5 — nomzod

Repo: https://github.com/justin6577-spec/InteractionSystem-UE5  
Litsenziya: MIT. README UE 5.4–5.8, C++20 va UE 5.8 compile tekshiruvini ko'rsatadi.

Funksiyalar: raycast/sphere trace, IInteractable interface, contextual prompt, highlight callback, Press/Hold/Toggle, hold progress, debug visualization va Blueprint API.

Ishlatish: eshik, pickup, NPC talk, do'kon peshtaxtasi, ish stansiyasi.  
Qaror: toza UE 5.8 test loyihasida compile/input testdan keyin qo'shish.

### 3.2 unreal-npc-simulation — faqat reference

Repo: https://github.com/Spart12Dmi/unreal-npc-simulation-  
UE 5.7 C++ loyiha. NPC schedule, simulyatsiya vaqti, schedule Data Asset, Gameplay Tags, interaction object reservation/capacity, Behavior Tree tasklari, harakat va animatsiya vazifalari bor.

**Litsenziya xavfi:** tekshirilgan LICENSE, LICENSE.md va LICENSE.txt yo'llarida litsenziya topilmadi. Muallif ruxsatisiz kodini ko'chirmaslik. Arxitekturasini o'rganib, o'z implementatsiyamizni yozish mumkin.

Docs: https://github.com/Spart12Dmi/unreal-npc-simulation-/tree/main/docs

### 3.3 UEproject — eski reference

Repo: https://github.com/TheEugen/UEproject  
Litsenziya: MIT. UE 5.0 o'quv/prototip. NPC daily routine, waypoint/pathfinding, dialog, inventory va item handling misollari bor. README'da lokal XML path va tugallanmagan quest scaffold haqida ogohlantirish mavjud.

Qaror: asosiy loyiha sifatida ishlatmaslik; faqat data layout va routine kodini o'rganish. Tanlangan kod UE 5.8 da alohida test qilinadi.

### 3.4 Inventory Plugin Community Version — nomzod

Repo: https://github.com/DanieltheDeveloper/inventory-plugin-community-version  
Litsenziya: MIT. README UE 5.5+ ko'rsatadi. C++ core, Blueprint extension pointlari, item Data Asset, container, stack, item drop, dynamic item properties, event/delegate va equipment imkoniyatlari bor.

Qaror: single-player minimal inventory use-case'ini UE 5.8 da test qilish. Keraksiz multiplayer/replication qismlarini hozircha ishlatmaslik.

### 3.5 UE5Inventory — economy reference

Repo: https://github.com/Synock/UE5Inventory  
Litsenziya: MIT. README'da grid inventory, item size/weight, currency, merchant cash limit, static/dynamic stock, buy/sell ratio, trading, banking, repair va durability bor.

Muallif o'zi loyiha o'rganish jarayonida yaratilgani, ayrim qarorlar xatoga moyil yoki samarasiz bo'lishi mumkinligini ogohlantiradi. UE 5.8 mosligi tekshirilishi kerak.

Qaror: merchant, stock, weight va currency qoidalarini o'rganish uchun. Ko'r-ko'rona import qilmaslik; kerakli qismini o'z economy arxitekturamizga moslab yozish.

### 3.6 Not Yet Dialogue System — nomzod

Repo: https://github.com/NotYetGames/DlgSystem  
Litsenziya: MIT. README UE 5.6, 5.7, 5.8 ni qo'llashini ko'rsatadi. Blueprint/C++, branching graph, event/condition, editor graph/search/browser, text/JSON import-export mavjud.

Qaror: UE 5.8 branch compile, dialog state save/restore va localization sinovidan keyin qo'shish.

### 3.7 SPUD — Save/Load nomzodi

Repo: https://github.com/sinbad/SPUD  
Litsenziya: MIT; UE5 uchun. Marked actorlar, SaveGame propertylar, array/struct/reference, spawn/destroy qilingan actorlar va level streaming persistence uchun foydali bo'lishi mumkin.

MVP'da avval Unreal USaveGame va async save ishlatiladi. SPUD faqat dynamic world persistence haqiqiy ehtiyojga aylansa qo'shiladi.

Rasmiy Save/Load: https://dev.epicgames.com/documentation/en-us/unreal-engine/saving-and-loading-your-game-in-unreal-engine  
SPUD FAQ: https://github.com/sinbad/SPUD/blob/master/doc/faq.md

## 4. 3D assetlar va animatsiyalar

Bular Fab'dagi topilgan nomzodlar. Yuklab olishdan oldin listingdagi ayni versiya, license va engine mosligini qayta tekshirish kerak.

- **Modular Urban Houses:** https://www.fab.com/listings/c6a42d9c-f2c5-4155-8ced-b1d6d1592f41  
  Listing: 10 ta tayyor bino, 40+ modular mesh, tileable tekstura va UE5 Lumen. Mahalla uchun nomzod.
- **50+ Interior Props & Furniture Starter Kit:** https://www.fab.com/listings/3230e8b4-a63a-44c6-8d8d-5ea3630c5627  
  Mebel, maishiy texnika va dekor; listing UE 5.8, Lumen/Nanite mosligini ko'rsatadi.
- **Motifect Daily Life Motion Pack:** https://www.fab.com/listings/bea29cd8-b9ca-47d3-87d5-acc571c39735  
  40 ta kundalik humanoid FBX animatsiya: o'tirish, turish, gapirish gesturelari, buyum olish/qo'yish, eshik ochish, qo'l silkitish. Skeleton retarget kerak.
- **Vehicle Variety Pack:** https://www.fab.com/listings/dc1ada50-2523-44b1-b0e2-a72d14076fb4  
  Listing 5 ta interiorli drivable vehicle ko'rsatadi. MVP'da faqat bittasini, piyoda gameplay loop barqaror bo'lgach ishlatish.
- **City Sample:** https://www.fab.com/listings/4898e707-7855-404b-af0e-a505ee690e68  
  Shahar, crowd, transport, World Partition, Nanite, Lumen, Chaos, Mass AI, MetaHuman, Niagara va MetaSounds reference. Butun sample og'ir; kerakli extracted packlarni alohida baholash.
- **FANTASTIC Village Pack:** https://www.fab.com/listings/52529a12-e88e-41a0-8834-b87306f20c24  
  400+ stylized asset; faqat stylized art direction tanlansa, realistik shahar assetlari bilan aralashtirmaslik.

Personaj manbalari:
- Third Person Template: https://dev.epicgames.com/documentation/unreal-engine/third-person-template-in-unreal-engine
- Mixamo FAQ: https://helpx.adobe.com/creative-cloud/faq/mixamo-faq.html
- MetaHuman license: https://www.metahuman.com/license

Dastlab bitta skeleton va 3–5 ko'rinish varianti. 15 ta film-sifat MetaHuman qilishdan oldin NPC schedule va performance isbotlansin.

## 5. Ovoz va materiallar

- Kenney Interface Sounds: https://kenney.nl/assets/interface-sounds
- Kenney UI Audio: https://kenney.nl/assets/ui-audio
- Kenney Impact Sounds: https://kenney.nl/assets/impact-sounds
- Kenney license: https://kenney.nl/support
- Poly Haven CC0: https://polyhaven.com/license
- Freesound: https://freesound.org/

Kenney asset sahifalaridagi audio CC0/public domain sifatida ko'rsatilgan. Poly Haven tekstura/HDRI/model CC0. Freesound'da har bir fayl litsenziyasi alohida: faqat CC0 yoki attribution talabiga rioya qila oladigan CC-BY. Non-commercial va NoDerivatives fayllarni release'ga qo'shmaslik.

Ovoz ro'yxati: ko'cha/uy ambience, beton/taxta/yer qadamlar, yomg'ir/shamol, eshik, interaction, UI feedback; NPC voice keyingi bosqichda.

## 6. O'zimiz yozadigan tizimlar

### Job System

2–3 kasb:
1. Do'kon yordamchisi/kassir: mijozga xizmat, mahsulotni berish, smena.
2. Yetkazib beruvchi: posilka olish, manzilga borish, topshirish.
3. Usta yordamchisi: material olib kelish, ish stansiyasida vazifa, mahsulotni topshirish.

Job Definition Data Asset: ID, nom, ish beruvchi, ish vaqti, kerakli skill, objective list, maosh, XP, fail/cancel, repeatability. Job Component: qabul qilingan ish, objective progress, shift state, reward claimed. Mukofot faqat bir marta berilishi shart.

### NPC Schedule

Schedule Data Asset: simulation time, activity Gameplay Tag, waypoint yoki Smart Object. NPC Schedule Component joriy activity tanlaydi; Behavior Tree yoki StateTree yo'l topish va task bajarishni boshqaradi. Reservation/capacity bitta stul/kassani ikki NPC egallashiga yo'l qo'ymaydi. Dastlab 3 NPC, keyin 10–15.

### Economy

Item: ID, nom, kategoriya, icon/mesh, base price, weight, stack limit, usable action. Shop: boshlang'ich stock, cash, buy/sell multiplier, restock. Talab oshsa narx asta ko'tariladi, zaxira ko'paysa pasayadi; narx base price atrofida clamp qilinadi.

Savdo tranzaksiyasi atomik bo'lsin: pul, inventar sig'imi va stock avval tekshiriladi, keyin hamma o'zgarishlar birgalikda qo'llanadi. Mavjud loyiha hujjatlaridagi Copper/Silver/Gold, inventory, trade, reputation va skill konsepsiyalari bilan uyg'unlashtirish; ikkinchi parallel economy yaratmaslik.

### World Time va real vaqt

- Simulation clock: o'yin kuni/soati/daqiqasi; NPC schedule, shop hours va job shiftni boshqaradi.
- Real UTC timestamp: save metadata va kelajakdagi offline progression uchun.

MVP'da NPC jadvali kompyuterning real soatiga bog'lanmasin. Simulyatsiya vaqti deterministic va test qilinadigan bo'lsin.

### Weather

Engine-native: Sky Atmosphere, Directional Light, Sky Light, Exponential Height Fog, WorldTimeSubsystem va WeatherSubsystem. Clear/cloudy/rain/fog preset, VFX/audio.

AT Dynamic Weather System nomzodi: https://www.fab.com/listings/4f21342d-8054-4208-8755-0c3236302513  
Listing UE 5.7 ko'rsatadi; UE 5.8 mosligi tekshirilmaguncha dependency qilinmaydi.

## 7. Save schema

Kamida: player transform, inventory/quantity, wallet/currency, job state/progress/XP, simulation date/time, shop stock/current price, kerakli NPC runtime state va o'yinchi o'zgartirgan muhim world objectlar.

Avval USaveGame/async save. Dynamic actor va level streaming persistence murakkablashsa SPUD baholanadi.

## 8. Test va optimizatsiya

Rasmiy manbalar:
- Automation Test Framework: https://dev.epicgames.com/documentation/en-us/unreal-engine/automation-test-framework-in-unreal-engine
- Gauntlet: https://dev.epicgames.com/documentation/unreal-engine/gauntlet-automation-framework-overview-in-unreal-engine
- Performance profiling: https://dev.epicgames.com/documentation/unreal-engine/introduction-to-performance-profiling-and-configuration-in-unreal-engine

Profiling: stat unit, stat game, stat gpu, Unreal Insights, Memory Insights va target PC'da packaged build.

Majburiy testlar:
1. Inventory: add/remove, stack, item use, weight/capacity.
2. Economy: buy/sell, pul yetmasligi, stock tugashi, inventory full, merchant cash limit.
3. Job: accept/progress/fail/cancel/complete, reward bir martadan ortiq berilmasligi.
4. Time: minute/hour/day rollover, pause va time scale.
5. NPC: schedule change, path reachable, reservation acquire/release, double booking yo'qligi.
6. Save/Load: player, wallet, inventory, job, clock, stock/prices va world state.
7. Build smoke: packaged game ishga tushadi, mahalla yuklanadi va gameplay loop bajariladi.

FPS maqsadi test kompyuteri va grafik sozlamalari belgilanmaguncha qat'iy raqam qilinmaydi.

## 9. Integratsiya tartibi

1. Foundation: UE 5.8 clean project, source control, Third Person Template, C++ module/folder, mahalla greybox.
2. Player + interaction: plugin test; eshik, pickup, NPC prompt.
3. Items + inventory: Data Asset, UI, stack, weight/capacity.
4. Time + NPC: clock, schedule, 3 NPC; keyin 10–15.
5. Shop + economy: bitta do'kon, 5 item, wallet, stock, transaction, dynamic price.
6. Jobs: bitta kasbni oxirigacha; keyin yana 1–2.
7. Save/Load: butun gameplay siklini saqlash va tiklash.
8. Content: bino, interior, character variants, retarget, audio, weather.
9. QA: automation, 30–60 daqiqalik playtest, packaged build va target PC profiling.

## 10. Litsenziya qoidalari

- MIT: copyright/license notice saqlash, ThirdPartyNotices.md yuritish.
- Fab: har bir listingning ayni license shartini tekshirish. Fab Standard License assetni o'yin tarkibida tarqatishga ruxsat beradi, ammo alohida asset sifatida qayta sotish/redistribute qilishga emas.
- CC0: tijoriy foydalanish mumkin; source/license manifestni saqlash.
- CC-BY: attribution talabiga rioya qilish.
- LICENSE fayli yo'q repo: ruxsatsiz kod ko'chirmaslik.
- Fab EULA: https://www.fab.com/eula
- Fab license types: https://dev.epicgames.com/documentation/fab/licenses-and-pricing

## 11. Hozirgi qaror

Baseline: UE 5.8 stable + Third Person Template; engine-native AI/navigation/Smart Objects/time/weather/SaveGame; MIT InteractionSystem, DlgSystem va inventory pluginlarini faqat alohida compile/testdan keyin qo'shish; SPUD faqat zarurat bo'lsa. Fab Modular Urban Houses, Interior Props, Motifect Daily Life Motion Pack, zarur bo'lsa bitta Vehicle Variety Pack; Kenney/Poly Haven CC0.

O'zimiz yozamiz: Job System, Economy rules, NPC Schedule logic, World Time, Weather orchestration va save schema.

## 12. Holat va keyingi vazifa

Ushbu katalog topilgan manbalar va ishlab chiqish rejasidir. Bu pluginlar import qilindi, kod compile bo'ldi, assetlar yuklandi, animatsiya retarget qilindi, testlar o'tdi yoki tayyor o'yin build qilindi degani EMAS.

Keyingi vazifa: UE 5.8 test loyihasida InteractionSystem, DlgSystem va Inventory pluginlarini bittadan compile/import qilib tekshirish. Har biri uchun engine version, commit/tag, license, compile natijasi, test natijasi va integratsiya qarorini qayd etish.

*Bu fayl Hayot Simulyatori MVP tadqiqot va integratsiya bo'yicha yagona katalog sifatida yangilanadi.*
