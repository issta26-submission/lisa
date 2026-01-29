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
//<ID> 1922
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
    cJSON *added_count = NULL;
    cJSON *num_item = NULL;
    cJSON *added_seven = NULL;
    cJSON *got_count = NULL;
    cJSON *got_seven = NULL;
    cJSON *got_enabled = NULL;
    char *out = NULL;
    double count_val = 0.0;
    double seven_val = 0.0;
    cJSON_bool has_count = 0;
    cJSON_bool enabled_true = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();

    // step 3: Core operations
    added_count = cJSON_AddNumberToObject(root, "count", 100.0);
    num_item = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "seven", num_item);
    cJSON_AddTrueToObject(root, "enabled");
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    got_count = cJSON_GetObjectItem(root, "count");
    got_seven = cJSON_GetObjectItem(root, "seven");
    has_count = cJSON_HasObjectItem(root, "count");
    got_enabled = cJSON_GetObjectItem(root, "enabled");
    enabled_true = cJSON_IsTrue(got_enabled);
    count_val = cJSON_GetNumberValue(got_count);
    seven_val = cJSON_GetNumberValue(got_seven);
    validation ^= (int)(root != NULL);
    validation ^= (int)(added_count != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(got_count != NULL);
    validation ^= (int)(got_seven != NULL);
    validation ^= (int)(got_enabled != NULL);
    validation ^= (int)(has_count != 0);
    validation ^= (int)(enabled_true != 0);
    validation ^= (int)(count_val == 100.0);
    validation ^= (int)(seven_val == 7.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}