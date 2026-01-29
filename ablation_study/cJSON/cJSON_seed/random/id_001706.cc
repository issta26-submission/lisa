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
//<ID> 1706
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
    cJSON *num_e = (cJSON *)0;
    char *json_unformatted = (char *)0;
    double val_pi = 0.0;
    double val_e = 0.0;
    int validation = 0;
    const char *key_pi = "pi";
    const char *key_e = "e";

    // step 2: Initialize
    root = cJSON_CreateObject();
    num_pi = cJSON_CreateNumber(3.14159);
    num_e = cJSON_CreateNumber(2.71828);

    // step 3: Configure
    cJSON_AddItemToObject(root, key_pi, num_pi);
    cJSON_AddItemToObject(root, key_e, num_e);

    // step 4: Operate
    val_pi = cJSON_GetNumberValue(cJSON_GetObjectItem(root, key_pi));
    val_e = cJSON_GetNumberValue(cJSON_GetObjectItem(root, key_e));
    json_unformatted = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation = (int)(val_pi > val_e) + (int)(json_unformatted != (char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_free(json_unformatted);
    cJSON_Delete(root);
    (void)root;
    (void)num_pi;
    (void)num_e;
    (void)json_unformatted;
    (void)val_pi;
    (void)val_e;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}