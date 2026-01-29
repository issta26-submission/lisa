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
//<ID> 1285
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *pi_item = NULL;
    cJSON *tmp = NULL;
    double pi_val = 0.0;
    double two_val = 0.0;
    double ref_val = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    pi_item = cJSON_CreateNumber(3.14159265);
    cJSON_AddItemToObject(root, "pi", pi_item);
    cJSON_AddNumberToObject(root, "two", 2.0);
    cJSON_AddNullToObject(root, "nothing");
    cJSON_AddItemReferenceToObject(root, "pi_ref", pi_item);

    // step 3: Operate / Validate
    tmp = cJSON_GetObjectItem(root, "pi");
    pi_val = cJSON_GetNumberValue(tmp);
    tmp = cJSON_GetObjectItem(root, "two");
    two_val = cJSON_GetNumberValue(tmp);
    tmp = cJSON_GetObjectItem(root, "pi_ref");
    ref_val = cJSON_GetNumberValue(tmp);
    (void)pi_val;
    (void)two_val;
    (void)ref_val;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}