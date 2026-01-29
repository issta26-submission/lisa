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
//<ID> 1595
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num = NULL;
    cJSON *nul = NULL;
    cJSON *flag = NULL;
    char *out = NULL;
    cJSON_bool flag_was_false = 0;
    double num_value = 0.0;
    const char *num_key = "num";
    const char *null_key = "n";
    const char *flag_key = "flag";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num = cJSON_CreateNumber(42.5);
    nul = cJSON_CreateNull();
    flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, num_key, num);
    cJSON_AddItemToObject(root, null_key, nul);
    cJSON_AddItemToObject(root, flag_key, flag);

    // step 3: Operate / Validate
    flag_was_false = cJSON_IsFalse(flag);
    num_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, num_key));
    cJSON_DeleteItemFromObject(root, flag_key);
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}