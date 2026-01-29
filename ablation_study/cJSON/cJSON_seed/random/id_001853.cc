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
//<ID> 1853
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
    cJSON *child = NULL;
    cJSON *num_item = NULL;
    cJSON *flag_item = NULL;
    cJSON *got_child = NULL;
    cJSON *got_num = NULL;
    cJSON *got_flag = NULL;
    char *out = NULL;
    double val = 0.0;
    cJSON_bool is_false = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.0);
    flag_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(child, "answer", num_item);
    cJSON_AddItemToObject(child, "flag", flag_item);
    cJSON_AddItemToObject(root, "nested", child);

    // step 3: Core operations
    got_child = cJSON_GetObjectItem(root, "nested");
    got_num = cJSON_GetObjectItem(got_child, "answer");
    val = cJSON_GetNumberValue(got_num);
    got_flag = cJSON_GetObjectItem(got_child, "flag");
    is_false = cJSON_IsFalse(got_flag);
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(child != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(flag_item != NULL);
    validation ^= (int)(got_child != NULL);
    validation ^= (int)(got_num != NULL);
    validation ^= (int)(got_flag != NULL);
    validation ^= (int)(val == 42.0);
    validation ^= (int)(is_false != 0);
    validation ^= (int)(out != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}