#include <stdlib.h>
#include <stdbool.h>

#include <gmptools.h>

#include "ffs.h"

static const char *domain_p_1024 = "10611883578308384411354608894331199883838299" \
                                   "88786854613844964970351946838349510066771477" \
                                   "70721373342273909479621515299896943660251488" \
                                   "56184090524165357943533";
static const char *domain_q_1024 = "11607089161902499241717501403185857077978903" \
                                   "89627507299476168199561181723925895302434779" \
                                   "79583470530023176383943222773126435444291432" \
                                   "83649655096152260372287";
static const char *domain_n_1024 = "12317307886915436029907316857785085068033671" \
                                   "89280390601605533929913628313151992076710544" \
                                   "92967266944403331521858069650979345399320429" \
                                   "83039397008537625401824415854965875939021691" \
                                   "15171468769070489793834564895996669995095931" \
                                   "36213882497964606727991015594524530630778845" \
                                   "322068122676106398018935220064598947304069971";

static const char *domain_p_2048 = "14989862801978795517059319599636935547689806" \
                                   "09972572115787852127040853112715435929361076" \
                                   "00284278223390447784786305104522126586035565" \
                                   "32678064815253343580866685122157060162548754" \
                                   "31794766171451781856563428231166818636457869" \
                                   "05046763994875015891635517403257259398381426" \
                                   "179410777039626458255107024549028940574768397";
static const char *domain_q_2048 = "16680913846261530735812766215194237528961350" \
                                   "03671842044448222929684674785190375685335458" \
                                   "33978299101372133019877800813225767208818392" \
                                   "48204321135606955004409832494955341155008940" \
                                   "84052067943303161768727360024367629668742917" \
                                   "11301320311310664937426167303324806753341242" \
                                   "214612545879365673520834330847861509779864217";
static const char *domain_n_2048 = "25004460996708875618725146141475935769674672" \
                                   "43951146927922368092184496742691159949907947" \
                                   "63568607404484582644061878843478514232252612" \
                                   "28507216784577488372099743380772816784886089" \
                                   "32100204246780404915489533274666816634717852" \
                                   "14720090511252824824678275275033107884530797" \
                                   "78709335575705907500916903121144135796387687" \
                                   "49665226109097218962911964964069989217582025" \
                                   "04818006914033407268172385267355102553465627" \
                                   "84805918019992260946903376837586697867853175" \
                                   "33497830186293915757553328614758755750216572" \
                                   "27465970432458619091449424437108063956958356" \
                                   "48160683124862163954125325281994550585319603" \
                                   "349906038292147441948191703257612358882750149";

static const char *domain_p_3072 = "17786359615874645437871233144226258894493521" \
                                   "52753302393848985853143003203194444801731986" \
                                   "28310049278281951916103380459701471365618627" \
                                   "89813819857138129559454345089818800841426585" \
                                   "16987321530555872870683520297474557940194205" \
                                   "34185350922085233763657857662129918985204959" \
                                   "25290459545712881358837958778635011278782541" \
                                   "94354202282331475531695229669716663740418728" \
                                   "81736493934651751237349852849343424960898139" \
                                   "95623525818076789423730132368527587802215398" \
                                   "52869699244616568222683";
static const char *domain_q_3072 = "23193557780423970705789433350363874069095702" \
                                   "57045285595707718227663023243947454467980427" \
                                   "43803041790780607062266663038611015424015823" \
                                   "12294838082813614268480939964252691934622911" \
                                   "01714768104391978442596567432258978915762857" \
                                   "09376924800629614811093333752723827648430301" \
                                   "31768913733340768213783857367416555434609191" \
                                   "31112086915094479961132565968351995032204140" \
                                   "69891118976993149586121867008485303298243763" \
                                   "26272871215093835229806996386502703466982101" \
                                   "58070892165519133616771";
static const char *domain_n_3072 = "41252895945418808964004689421071873905396007" \
                                   "80608481268077420518169878542544890080576339" \
                                   "51782757100677289943394643261405442158748333" \
                                   "79006735171658989819467076956565862319838521" \
                                   "20329184822531276865819175556079987808340481" \
                                   "89599472558077020153884782311951061599844096" \
                                   "42270677359423886135371240672778995791091688" \
                                   "78563394885525718705048627558163776457852997" \
                                   "71753459441610006804719840888552871868411910" \
                                   "03529717048116529775581061397581113492143427" \
                                   "52700751015380921656766137136329962735165299" \
                                   "52492243880366172170189523382269394891221284" \
                                   "59485717104050560033387376842497944612548079" \
                                   "44786966698737322854729481183344171615605418" \
                                   "13582900867096552704349783790180998828921028" \
                                   "49559271866982181626270553462118834353354314" \
                                   "55435908148401748639547044482823796393557964" \
                                   "62401231760097444620500523554586127022278101" \
                                   "35475216229060077386971369499212327580687732" \
                                   "27797932642445365354059638316712654073887219" \
                                   "877816242933304171902343005691246493111416593";

static inline void random_prime(mpz_t prime, size_t bit_size)
{
        do {
                gmpt_random(prime, bit_size);
        } while (!gmpt_isprime(prime));
}


_Bool ffs_init(struct ffs_params *params, size_t bit_size)
{
        if (bit_size != FFS_NLEN_1024 && bit_size != FFS_NLEN_2048 && bit_size != FFS_NLEN_3072)
                return false;

        switch (bit_size) {
        case 1024:
                params->k = FFS_KMAX;
                params->t = 10;
                mpz_init_set_str(params->p, domain_p_1024, 10);
                mpz_init_set_str(params->q, domain_q_1024, 10);
                mpz_init_set_str(params->n, domain_n_1024, 10);
                return true;
        case 2048:
                params->k = FFS_KMAX;
                params->t = 11;
                mpz_init_set_str(params->p, domain_p_2048, 10);
                mpz_init_set_str(params->q, domain_q_2048, 10);
                mpz_init_set_str(params->n, domain_n_2048, 10);
                return true;
        case 3072:
                params->k = FFS_KMAX;
                params->t = 12;
                mpz_init_set_str(params->p, domain_p_3072, 10);
                mpz_init_set_str(params->q, domain_q_3072, 10);
                mpz_init_set_str(params->n, domain_n_3072, 10);
                return true; 
        }
        perror("wrong bit length");
        return false;
}


void ffs_keys_create(struct ffs_member_params *params, mpz_t n)
{
        for (int i = 0; i < FFS_KMAX; i++) {
                mpz_init(params->s_arr[i]);
                mpz_init(params->i_arr[i]);
                gmpt_rndmod(params->s_arr[i], n);
                mpz_pow_ui(params->i_arr[i], params->s_arr[i], 2);
                mpz_invert(params->i_arr[i], params->i_arr[i], n);
        }
}


void ffs_keys_destroy(struct ffs_member_params *params)
{
        for (int i = 0; i < FFS_KMAX; i++) {
                mpz_clear(params->s_arr[i]);
                mpz_clear(params->i_arr[i]);
        }
}


void ffs_commitment(mpz_t X, mpz_t R, mpz_t n)
{
        mpz_t rndbit;
        mpz_init(rndbit);

        gmpt_random(rndbit, 1);

        gmpt_rndmod(R, n);
        mpz_powm_ui(X, R, 2, n);
        
        if (mpz_tstbit(rndbit, 0))
                mpz_mul_si(X, X, -1);

        mpz_clear(rndbit);
}

void ffs_solve(struct ffs_member_params *params, mpz_t Y, mpz_t E, mpz_t R, mpz_t n)
{
        mpz_set(Y, R);
        for (int i = 0; i < FFS_KMAX; i++) {
                if (mpz_tstbit(E, i))
                        mpz_mul(Y, Y, params->s_arr[i]);
        }
        mpz_mod(Y, Y, n);
}


_Bool ffs_verify(mpz_t i_arr[FFS_KMAX], mpz_t X, mpz_t Y, mpz_t E, mpz_t n)
{
        _Bool result = true;
        mpz_t real_x;
        mpz_init(real_x);
        mpz_pow_ui(real_x, Y, 2);

        for (int i = 0; i < FFS_KMAX; i++) {
                if (mpz_tstbit(E, i))
                        mpz_mul(real_x, real_x, i_arr[i]);
        }
        mpz_mod(real_x, real_x, n);

        if (mpz_cmpabs(X, real_x) != 0)
                result = false;

        mpz_clear(real_x);
        return result;
}


void ffs_free(struct ffs_params *params)
{
        mpz_clear(params->q);
        mpz_clear(params->p);
        mpz_clear(params->n);
}