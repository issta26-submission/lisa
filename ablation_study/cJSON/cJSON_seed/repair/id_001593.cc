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
//<ID> 1593
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);

    // step 2: Configure
    cJSON *threshold = cJSON_AddNumberToObject(config, "threshold", 12.5);
    cJSON *name_str = cJSON_CreateString("sensor-A");
    cJSON_AddItemToObject(config, "name", name_str);
    cJSON *mult = cJSON_CreateNumber(2.0);
    cJSON_bool ref_added1 = cJSON_AddItemReferenceToObject(root, "multiplier_ref", mult);
    cJSON *label = cJSON_CreateString("root_label");
    cJSON_bool ref_added2 = cJSON_AddItemReferenceToObject(root, "label_ref", label);

    // step 3: Operate and Validate
    cJSON *got_config = cJSON_GetObjectItemCaseSensitive(root, "config");
    cJSON *got_threshold = cJSON_GetObjectItemCaseSensitive(got_config, "threshold");
    double thr_val = cJSON_GetNumberValue(got_threshold);
    cJSON *got_multiplier = cJSON_GetObjectItemCaseSensitive(root, "multiplier_ref");
    double mult_val = cJSON_GetNumberValue(got_multiplier);
    double product = thr_val * mult_val;
    cJSON *product_item = cJSON_CreateNumber(product);
    cJSON_AddItemToObject(root, "product", product_item);
    char scratch[64];
    memset(scratch, 0, sizeof(scratch));
    scratch[0] = (char)('0' + (int)ref_added1);
    scratch[1] = (char)('0' + (int)ref_added2);
    scratch[2] = (char)('0' + (int)(product_item != NULL));

    // step 4: Cleanup
    cJSON_Delete(root);
    (void)scratch;
    return 66;
    // API sequence test completed successfully
}