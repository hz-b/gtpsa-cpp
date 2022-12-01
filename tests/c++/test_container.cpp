    //
    // Created by mfp on 24.11.22.
    //
    #define BOOST_TEST_MODULE gtpsa_container
    #define BOOST_TEST_DYN_LINK
    #include <boost/test/unit_test.hpp>
    #include <gtpsa/utils.hpp>
    //#include <gtpsa/gtpsa_container.h>
    //#include <gtpsa/ss_vect.h>

    struct Base {
    public:
        int flag = 0;
    };
    struct Derived : public Base {};

    BOOST_AUTO_TEST_CASE(test01_vector_cast)
    {
        std::vector<Derived> vec;

        size_t n_elem = 10;
        for(size_t cnt = 0; cnt <n_elem; cnt++) {vec.push_back(Derived());}
        for(size_t cnt = 0; cnt < vec.size(); ++cnt){
            vec[cnt].flag = cnt +1 ;
        }
        auto v2 = gtpsa::vector_type_to_base_ptr<Derived, Base>(vec);

        BOOST_CHECK_EQUAL(v2[2]->flag, 3);
        BOOST_CHECK_EQUAL(v2[9]->flag, 10);
    }

    BOOST_AUTO_TEST_CASE(test02_vector_const_cast)
    {
        std::vector<Derived> tmp;
        for(size_t cnt = 0; cnt <10; cnt++) {tmp.push_back(Derived());}
        int cnt=1;
        for(auto & e : tmp){e.flag = cnt++;}
        const std::vector<Derived> vec(tmp);


        auto v2 = gtpsa::vector_type_to_base_ptr<Derived, Base>(vec);

        BOOST_CHECK_EQUAL(v2[0]->flag, 1);
        BOOST_CHECK_EQUAL(v2[9]->flag, 10);

    }

    BOOST_AUTO_TEST_CASE(test03_vector__ptr_cast)
    {
        std::vector<Derived> tmp;

        size_t n_elem = 10;
        for(size_t cnt = 0; cnt <n_elem; cnt++) {tmp.push_back(Derived());}
        int cnt=1;
        for(auto & e : tmp){e.flag = cnt++;}
        auto vec = gtpsa::vector_type_to_base_ptr<Derived, Derived>(tmp);
        auto v2 = gtpsa::vector_ptr_to_base_ptr<Derived, Base>(vec);

        BOOST_CHECK_EQUAL(v2[0]->flag, 1);
        BOOST_CHECK_EQUAL(v2[9]->flag, 10);
    }

    BOOST_AUTO_TEST_CASE(test05_vector_ptr_const_cast)
    {
        std::vector<Derived> tmp;
        for(size_t cnt = 0; cnt <10; cnt++) {tmp.push_back(Derived());}
        int cnt=1;
        for(auto & e : tmp){e.flag = cnt++;}
        const std::vector<Derived> tmp2(tmp);


        auto vec = gtpsa::vector_type_to_base_ptr<Derived, Derived>(tmp2);
        auto v2 = gtpsa::vector_ptr_to_base_ptr<Derived, Base>(vec);

        BOOST_CHECK_EQUAL(v2[0]->flag, 1);
        BOOST_CHECK_EQUAL(v2[9]->flag, 10);

    }

    int test_func(const Base buf[], size_t n_elem){

    }
    BOOST_AUTO_TEST_CASE(test20_buffer)
    {
        std::vector<Derived> tmp;
        for(size_t cnt = 0; cnt <10; cnt++) {tmp.push_back(Derived());}
        int cnt=10;
        for(auto & e : tmp){e.flag = (2*cnt++);}
        const std::vector<Derived> tmp2(tmp);

        test_func(tmp2.data(), tmp2.size() );
    }
#if 0
    BOOST_AUTO_TEST_CASE(test60_compose)
            {
                    const auto desc = std::make_shared<gtpsa::desc> (6, 2);
                    gtpsa::ss_vect<gtpsa::tpsa> ma(desc, 1), mb(desc, 1);
                    ma.setName("ma");
                    mb.setName("mb");

                    arma::mat identity(6,6, arma::fill::eye);
                    ma.setJacobian(identity);
                    mb.setJacobian(identity);

                    gtpsa::Container<gtpsa::TpsaTypeInfo> ma_c(ma.getVector()), mb_c(mb.getVector());


                    auto mc = gtpsa::compose(ma_c, mb_c);

            }
    #endif