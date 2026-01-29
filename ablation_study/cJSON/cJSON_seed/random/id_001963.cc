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
//<ID> 1963
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
    cJSON *got_str = NULL;
    const char *got_str_val = NULL;
    char *heap_buf = NULL;
    double num_val = 0.0;
    cJSON_bool added_bool = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(123.5);
    str_item = cJSON_CreateString("embedded");
    cJSON_AddItemToObject(root, "value", num_item);
    cJSON_AddItemToObject(root, "label", str_item);
    bool_item = cJSON_AddBoolToObject(root, "active", 1);

    // step 3: Core operations
    got_num = cJSON_GetObjectItem(root, "value");
    num_val = cJSON_GetNumberValue(got_num);
    got_str = cJSON_GetObjectItem(root, "label");
    got_str_val = cJSON_GetStringValue(got_str);
    heap_buf = (char *)cJSON_malloc((got_str_val != NULL ? (strlen(got_str_val) + 1) : 1));
    if (heap_buf != NULL && got_str_val != NULL) {
        memcpy(heap_buf, got_str_val, strlen(got_str_val) + 1);
    }

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(bool_item != NULL);
    validation ^= (int)(got_num != NULL);
    validation ^= (int)(got_str != NULL);
    validation ^= (int)(got_str_val != NULL);
    validation ^= (int)(heap_buf != NULL);
    validation ^= (int)(num_val == 123.5);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(heap_buf);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}