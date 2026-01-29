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
//<ID> 1964
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[64];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *str_item = NULL;
    cJSON *bool_item = NULL;
    cJSON *got_num = NULL;
    double num_val = 0.0;
    char *dyn_str = NULL;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", num_item);
    dyn_str = (char *)cJSON_malloc((size_t)32);
    dyn_str[0] = 'h';
    dyn_str[1] = 'i';
    dyn_str[2] = 0;
    str_item = cJSON_CreateString(dyn_str);
    cJSON_AddItemToObject(root, "text", str_item);
    bool_item = cJSON_AddBoolToObject(root, "valid", (cJSON_bool)1);

    // step 3: Core operations
    got_num = cJSON_GetObjectItem(root, "answer");
    num_val = cJSON_GetNumberValue(got_num);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(bool_item != NULL);
    validation ^= (int)(got_num != NULL);
    validation ^= (int)(num_val == 42.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(dyn_str);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}