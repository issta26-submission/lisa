#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 1787
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *inner = (cJSON *)0;
    cJSON *s = (cJSON *)0;
    cJSON *n = (cJSON *)0;
    cJSON *alias_ref = (cJSON *)0;
    cJSON *retrieved_alias = (cJSON *)0;
    cJSON *greet_item = (cJSON *)0;
    cJSON *pi_item = (cJSON *)0;
    char *greet = (char *)0;
    double pi_value = 0.0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    inner = cJSON_CreateObject();
    s = cJSON_CreateString("world");
    n = cJSON_CreateNumber(3.1415);

    // step 3: Configure
    add_ok1 = cJSON_AddItemToObject(inner, "greet", s);
    add_ok2 = cJSON_AddItemToObject(inner, "pi", n);
    add_ok3 = cJSON_AddItemToObject(root, "inner", inner);
    cJSON_AddItemReferenceToObject(root, "alias", inner);

    // step 4: Operate
    retrieved_alias = cJSON_GetObjectItem(root, "alias");
    greet_item = cJSON_GetObjectItem(retrieved_alias, "greet");
    pi_item = cJSON_GetObjectItem(retrieved_alias, "pi");
    greet = cJSON_GetStringValue(greet_item);
    pi_value = cJSON_GetNumberValue(pi_item);

    // step 5: Validate
    validation = (int)(greet != (char *)0) + (int)(pi_value == 3.1415) + (int)(add_ok1 != 0) + (int)(add_ok2 != 0) + (int)(add_ok3 != 0);
    (void)validation;

    // step 6: Cleanup
    alias_ref = cJSON_DetachItemFromObjectCaseSensitive(root, "alias");
    cJSON_Delete(alias_ref);
    cJSON_Delete(root);
    (void)root;
    (void)inner;
    (void)s;
    (void)n;
    (void)retrieved_alias;
    (void)greet_item;
    (void)pi_item;
    (void)greet;
    (void)pi_value;
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}