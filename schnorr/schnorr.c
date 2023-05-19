#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <x86intrin.h>

#include "schnorr.h"

static const char *domain_p_1024 = "161112292587095383147111851630730203802946564227034344414818646092057864247055077" \
                                   "679093499921094739301225382398908958061735553955466513209613733995434925874891412" \
                                   "738360501525326816106752547847150547416769330496830642708513198285202845522662839" \
                                   "283441419660157241151647586466461617229119468446853874997369840871";

static const char *domain_q_1024 = "835221576128203481571055433189270505395861304749";

static const char *domain_g_1024 = "547352557045899734528764941224565267507202482872511422011420179614906932636419521" \
                                   "698316069410093885756111025457840337171890390941977964399784289164092133357059846" \
                                   "778673491885610310577221615857913349126023913143169074007681494712243396266406796" \
                                   "31241473365443625428083721236016636803218977448332449900922524459";

static const char *domain_p_2048 = "228493609264978586710447022740809387891405787617730394382628543610229768850437714" \
                                   "551602462609538887867148888086888808327634394607664786137399174206451284988482209" \
                                   "597518276175935187324132669701813298189258799282546716817042993798827549491715755" \
                                   "210927326202079007388615610452784456599809002271305714663104332617745673770942206" \
                                   "228817131697943415227247136099085356100857850909965289589348119012259233572080733" \
                                   "293565258883012053199955934654342012934336978687924327141304018893759467103458436" \
                                   "703800938614030771101013442480242043491629067403062092648255872886154754853165837" \
                                   "40594114913315487724426429255738617107153209341771";

static const char *domain_q_2048 = "26203072578963981314773993106128789217965297796936120286474251342221";

static const char *domain_g_2048 = "128482833730112280341282200208873402577966546388146396657386115902287659612597982" \
                                   "378609424889368208886341013948035396718286504484481445150017844724547505706467182" \
                                   "766691349150265596957392754715792479361606049423596969861981745334416501449417039" \
                                   "469089864308429666705774084744351333150175709732680178397042772273400317558356653" \
                                   "356165717271858156397141183650881297254200752452075475515406681323079768198165161" \
                                   "356457969994168625012013785429183533720415592540265263002324230101301300656519611" \
                                   "661442494491300315058112800781367129431323631547309176605774353736690623487755769" \
                                   "04156436410620092290470826399174382281178136328406";

static const char *domain_p_3072 = "486296439162085610705256667300749155252254860836921544325307641169991662063889080" \
                                   "633788130744198752746601430353260597033758159575351155596874730598894596819514534" \
                                   "289207179137059271061489129462484102916470509181509160970191889147737256117721664" \
                                   "734709460907976017049842182855515876554865980972820643956706773432136376206561496" \
                                   "875684848925128229332390146727115366410683712270431168206744586223565521687811916" \
                                   "609326737024287190944060044841474678116015416443190737577060264541754781595787544" \
                                   "713922976067102936338158440841769272963322426551209232195834617247040078666295705" \
                                   "031366952138119986477677822063754300142830243379954586717379576187768667029000413" \
                                   "409079890915841750548963765522686366872245215428875449993898215485302196188205475" \
                                   "268571869454047583575831430113512475901721323718520822443974080638244050011420213" \
                                   "461199202817479192515751147628605519738938770231658471439651651632416491452869572" \
                                   "4567826739438644325503806450716137";

static const char *domain_q_3072 = "89026556425181242887855588496172677611686960359969485652472817472597403586931";

static const char *domain_g_3072 = "236722043059822962275507462480506123933501556292320409764406367491062190604763702" \
                                   "986262886977441472656403831953610136506130513253952225042618391212247688630618146" \
                                   "295516702067413035042548546943434638780828272125890872872897527468659364454043616" \
                                   "221417542144434429574637375075114165311713585664490310002622354721001464272981512" \
                                   "124088302242873495407490984710313534707087850402844083132828727040916404283891176" \
                                   "051880933023100534342439544762106628369290818679312495052218913079702404357765828" \
                                   "571945442726094230090311564777557650925517254145132068333437995005326400055393936" \
                                   "311152349748145736079405530958216749274402307162674696532523130702050593781727175" \
                                   "915626971750605836335241520973596003459658894862963798639129355470282432172918415" \
                                   "090752549946453537885785536677198605499391933813994935784415763063582172303783988" \
                                   "886875203175798639680029558799398150106284415743378141933311678997840668039536152" \
                                   "5500300247621820794190978482526796";

_Bool schnorr_init(struct schnorr_params *params, size_t bit_size)
{
        switch (bit_size) {
        case 1024:
                mpz_init_set_str(params->p, domain_p_1024, 10);
                mpz_init_set_str(params->q, domain_q_1024, 10);
                mpz_init_set_str(params->g, domain_g_1024, 10);
                return true;
        case 2048:
                mpz_init_set_str(params->p, domain_p_2048, 10);
                mpz_init_set_str(params->q, domain_q_2048, 10);
                mpz_init_set_str(params->g, domain_g_2048, 10);
                return true;
        case 3072:
                mpz_init_set_str(params->p, domain_p_3072, 10);
                mpz_init_set_str(params->q, domain_q_3072, 10);
                mpz_init_set_str(params->g, domain_g_3072, 10);
                return true; 
        }
        perror("wrong bit length");
        return false;
}

void schnorr_free(struct schnorr_params *params)
{
        mpz_clear(params->p);
        mpz_clear(params->q);
        mpz_clear(params->g);
}


void schnorr_random(mpz_t random, size_t bitlen)
{
        const size_t dword_bits = sizeof(uint32_t) * CHAR_BIT;
        while (bitlen > 0) {
                uint32_t step_rnd = 0;
                while (_rdseed32_step(&step_rnd) == 0);

                size_t step_bits = (bitlen < dword_bits) ? bitlen : dword_bits;
                step_rnd >>= dword_bits - step_bits;
                bitlen -= step_bits;

                mpz_mul_2exp(random, random, step_bits);
                mpz_add_ui(random, random, step_rnd);
        }
}

/* get uniform random value 0 < random < mod */
void schnorr_rndmod(mpz_t random, mpz_t mod)
{
        do {
                size_t bitlen = mpz_sizeinbase(mod, 2);
                schnorr_random(random, bitlen);
        } while (mpz_cmp_ui(random, 0) == 0);
}


void schnorr_import(mpz_t dest, const void *int_data, uint32_t count)
{
        mpz_import(dest, count, 1, 1, 0, 0, int_data);
}


uint8_t *schnorr_export(const mpz_t src, uint32_t *count)
{
        const uint32_t byte_base = 0x100;
        *count = mpz_sizeinbase(src, byte_base);
        uint8_t *buffer = malloc(*count);
        mpz_export(buffer, NULL, 1, 1, 0, 0, src);
        return buffer;
}


void schnorr_user_keys(mpz_t s, mpz_t v, struct schnorr_params *params)
{
        schnorr_rndmod(s, params->q);
        /* v = g ^ -s mod p */
        mpz_sub(v, params->q, s);
        mpz_powm(v, params->g, v, params->p);
}

/* prover preprocessing step, produces 
 * secret random @r E [1, ..,q-1]
 * shared @x = g ^ r mod p */
void schnorr_preprocess(struct schnorr_params *params, mpz_t x, mpz_t r)
{
        schnorr_rndmod(r, params->q);

        mpz_powm(x, params->g, r, params->p);
}

/* 0 < @e < q is intended to be provided by B as a challange to be signed.
 * @s is A private key
 * @r - provers private random generated
 * @y = r + se (mod q) - result */
void schnorr_sign(struct schnorr_params *params, mpz_t y, mpz_t r, mpz_t s, mpz_t e)
{
        mpz_mul(y, s, e);
        mpz_add(y, y, r);
        mpz_mod(y, y, params->q);
}

void schnorr_verify(struct schnorr_params *params, mpz_t x_ver, mpz_t y, mpz_t v, mpz_t e)
{
        mpz_t g_pow_y, v_pow_e;
        mpz_init(g_pow_y);
        mpz_init(v_pow_e);
        mpz_powm(g_pow_y, params->g, y, params->p);
        mpz_powm(v_pow_e, v, e, params->p);
        mpz_mul(x_ver, g_pow_y, v_pow_e);
        mpz_mod(x_ver, x_ver, params->p);
}