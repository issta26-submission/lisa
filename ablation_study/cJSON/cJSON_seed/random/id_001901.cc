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
//<ID> 1901
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
    cJSON *num_manual = NULL;
    cJSON *num_via_add = NULL;
    cJSON *got_num_manual = NULL;
    cJSON *got_num_via_add = NULL;
    cJSON *got_bool = NULL;
    char *out = NULL;
    double val_manual = 0.0;
    double val_via_add = 0.0;
    cJSON_bool bool_flag = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_manual = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToObject(root, "pi_manual", num_manual);
    num_via_add = cJSON_AddNumberToObject(root, "pi_direct", 2.71828);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Core operations
    got_num_manual = cJSON_GetObjectItem(root, "pi_manual");
    got_num_via_add = cJSON_GetObjectItem(root, "pi_direct");
    got_bool = cJSON_GetObjectItem(root, "enabled");
    val_manual = cJSON_GetNumberValue(got_num_manual);
    val_via_add = cJSON_GetNumberValue(got_num_via_add);
    bool_flag = cJSON_IsTrue(got_bool);
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_manual != NULL);
    validation ^= (int)(num_via_add != NULL);
    validation ^= (int)(got_num_manual != NULL);
    validation ^= (int)(got_num_via_add != NULL);
    validation ^= (int)(got_bool != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(val_manual > 3.0);
    validation ^= (int)(val_via_add > 2.0);
    validation ^= (int)(bool_flag != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}