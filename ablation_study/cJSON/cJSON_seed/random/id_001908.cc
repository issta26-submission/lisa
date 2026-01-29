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
//<ID> 1908
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
    cJSON *manual_num = NULL;
    cJSON *auto_num = NULL;
    cJSON *bool_item = NULL;
    cJSON *got_manual = NULL;
    cJSON *got_auto = NULL;
    cJSON *got_bool = NULL;
    char *out = NULL;
    cJSON_bool is_num_manual = 0;
    cJSON_bool is_num_auto = 0;
    cJSON_bool is_bool = 0;
    double val_manual = 0.0;
    double val_auto = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    manual_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "manual_number", manual_num);
    auto_num = cJSON_AddNumberToObject(root, "auto_number", 3.1415);
    bool_item = cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Core operations
    got_manual = cJSON_GetObjectItem(root, "manual_number");
    got_auto = cJSON_GetObjectItem(root, "auto_number");
    got_bool = cJSON_GetObjectItem(root, "enabled");
    is_num_manual = cJSON_IsNumber(got_manual);
    is_num_auto = cJSON_IsNumber(got_auto);
    is_bool = cJSON_IsBool(got_bool);
    val_manual = cJSON_GetNumberValue(got_manual);
    val_auto = cJSON_GetNumberValue(got_auto);
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(manual_num != NULL);
    validation ^= (int)(auto_num != NULL);
    validation ^= (int)(bool_item != NULL);
    validation ^= (int)(got_manual != NULL);
    validation ^= (int)(got_auto != NULL);
    validation ^= (int)(got_bool != NULL);
    validation ^= (int)(is_num_manual != 0);
    validation ^= (int)(is_num_auto != 0);
    validation ^= (int)(is_bool != 0);
    validation ^= (int)(val_manual == 42.0);
    validation ^= (int)(val_auto != 0.0);
    validation ^= (int)(out != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}