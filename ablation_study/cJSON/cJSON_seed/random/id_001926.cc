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
//<ID> 1926
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
    cJSON *added_age = NULL;
    cJSON *num_created = NULL;
    cJSON *got_age = NULL;
    cJSON *got_enabled = NULL;
    char *out = NULL;
    double age_val = 0.0;
    cJSON_bool has_age = 0;
    cJSON_bool enabled_true = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    added_age = cJSON_AddNumberToObject(root, "age", 30.5);
    num_created = cJSON_CreateNumber(99.0);
    cJSON_AddItemToObject(root, "answer", num_created);
    cJSON_AddTrueToObject(root, "enabled");

    // step 3: Core operations
    has_age = cJSON_HasObjectItem(root, "age");
    got_age = cJSON_GetObjectItem(root, "age");
    age_val = cJSON_GetNumberValue(got_age);
    got_enabled = cJSON_GetObjectItem(root, "enabled");
    enabled_true = cJSON_IsTrue(got_enabled);
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(added_age != NULL);
    validation ^= (int)(num_created != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(has_age != 0);
    validation ^= (int)(got_age != NULL);
    validation ^= (int)(got_enabled != NULL);
    validation ^= (int)(cJSON_IsNumber(got_age));
    validation ^= (int)(age_val == 30.5);
    validation ^= (int)(enabled_true != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}