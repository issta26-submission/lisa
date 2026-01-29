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
//<ID> 1925
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
    cJSON *age_item = NULL;
    cJSON *num_created = NULL;
    cJSON *lucky_item = NULL;
    cJSON *true_item = NULL;
    cJSON *got_age = NULL;
    cJSON *got_lucky = NULL;
    cJSON *got_active = NULL;
    char *out = NULL;
    double age_val = 0.0;
    double lucky_val = 0.0;
    cJSON_bool has_age = 0;
    cJSON_bool is_active = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    age_item = cJSON_AddNumberToObject(root, "age", 25.0);
    num_created = cJSON_CreateNumber(77.0);
    cJSON_AddItemToObject(root, "lucky", num_created);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", true_item);
    out = cJSON_PrintUnformatted(root);

    // step 3: Core operations
    has_age = cJSON_HasObjectItem(root, "age");
    got_age = cJSON_GetObjectItem(root, "age");
    got_lucky = cJSON_GetObjectItem(root, "lucky");
    got_active = cJSON_GetObjectItem(root, "active");
    is_active = cJSON_IsTrue(got_active);
    age_val = cJSON_GetNumberValue(got_age);
    lucky_val = cJSON_GetNumberValue(got_lucky);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(age_item != NULL);
    validation ^= (int)(num_created != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(has_age != 0);
    validation ^= (int)(got_age != NULL);
    validation ^= (int)(got_lucky != NULL);
    validation ^= (int)(got_active != NULL);
    validation ^= (int)(is_active != 0);
    validation ^= (int)(age_val == 25.0);
    validation ^= (int)(lucky_val == 77.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}