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
//<ID> 1704
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num_pi = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    char *unformatted = (char *)0;
    double pi_value = 0.0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    num_pi = cJSON_CreateNumber(3.1415);
    str_item = cJSON_CreateString("circle");

    // step 3: Configure
    add_ok1 = cJSON_AddItemToObject(root, "pi", num_pi);
    add_ok2 = cJSON_AddItemToObject(root, "name", str_item);

    // step 4: Operate
    unformatted = cJSON_PrintUnformatted(root);
    pi_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "pi"));

    // step 5: Validate
    validation = (int)(add_ok1) + (int)(add_ok2) + (int)(pi_value == 3.1415) + (int)(unformatted != (char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(root);
    (void)root;
    (void)num_pi;
    (void)str_item;
    (void)unformatted;
    (void)pi_value;
    (void)add_ok1;
    (void)add_ok2;
    // API sequence test completed successfully
    return 66;
}