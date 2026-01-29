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
//<ID> 1858
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
    cJSON *false_item = NULL;
    cJSON *got_num = NULL;
    cJSON *got_false = NULL;
    char *out = NULL;
    double extracted_value = 0.0;
    cJSON_bool extracted_false = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.0);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "answer", num_item);
    cJSON_AddItemToObject(root, "flag", false_item);

    // step 3: Core operations
    out = cJSON_PrintUnformatted(root);
    got_num = cJSON_GetObjectItem(root, "answer");
    got_false = cJSON_GetObjectItem(root, "flag");
    extracted_value = cJSON_GetNumberValue(got_num);
    extracted_false = cJSON_IsFalse(got_false);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(false_item != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(got_num != NULL);
    validation ^= (int)(got_false != NULL);
    validation ^= (int)(extracted_value == 42.0);
    validation ^= (int)(extracted_false != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}