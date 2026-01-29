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
//<ID> 1900
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
    cJSON *auto_num_item = NULL;
    cJSON *manual_num_item = NULL;
    cJSON *got_auto = NULL;
    cJSON *got_manual = NULL;
    cJSON *got_active = NULL;
    char *out = NULL;
    cJSON_bool active_flag = 0;
    double val_auto = 0.0;
    double val_manual = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "score_auto", 42.5);
    manual_num_item = cJSON_CreateNumber(100.0);
    cJSON_AddItemToObject(root, "score_manual", manual_num_item);
    cJSON_AddBoolToObject(root, "active", 1);

    // step 3: Core operations
    got_auto = cJSON_GetObjectItem(root, "score_auto");
    val_auto = cJSON_GetNumberValue(got_auto);
    got_manual = cJSON_GetObjectItem(root, "score_manual");
    val_manual = cJSON_GetNumberValue(got_manual);
    got_active = cJSON_GetObjectItem(root, "active");
    active_flag = cJSON_IsBool(got_active) ? 1 : 0;
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(manual_num_item != NULL);
    validation ^= (int)(got_auto != NULL);
    validation ^= (int)(got_manual != NULL);
    validation ^= (int)(got_active != NULL);
    validation ^= (int)(val_auto == 42.5);
    validation ^= (int)(val_manual == 100.0);
    validation ^= (int)(active_flag != 0);
    validation ^= (int)(out != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}