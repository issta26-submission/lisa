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
//<ID> 1960
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
    char *str_val = NULL;
    void *mem = NULL;
    size_t len = 0;
    double num_val = 0.0;
    cJSON_bool added_value = 0;
    cJSON_bool added_name = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.0);
    added_value = cJSON_AddItemToObject(root, "value", num_item);
    bool_item = cJSON_AddBoolToObject(root, "flag", 1);
    str_item = cJSON_CreateString("hello");
    added_name = cJSON_AddItemToObject(root, "name", str_item);

    // step 3: Core operations
    got_num = cJSON_GetObjectItem(root, "value");
    num_val = cJSON_GetNumberValue(got_num);
    str_val = cJSON_GetStringValue(str_item);
    len = (str_val != NULL) ? (strlen(str_val) + 1) : 0;
    mem = (len > 0) ? cJSON_malloc(len) : NULL;
    if (mem != NULL && str_val != NULL) memcpy(mem, str_val, len);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(bool_item != NULL);
    validation ^= (int)(added_value != 0);
    validation ^= (int)(added_name != 0);
    validation ^= (int)(num_val == 42.0);
    validation ^= (int)(mem != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(mem);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}