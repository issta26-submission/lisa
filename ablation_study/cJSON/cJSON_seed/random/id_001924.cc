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
//<ID> 1924
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
    cJSON *added_pi = NULL;
    cJSON *created_hundred = NULL;
    cJSON *got_pi = NULL;
    cJSON *got_hundred = NULL;
    cJSON *got_enabled = NULL;
    char *out = NULL;
    double pi_val = 0.0;
    double hundred_val = 0.0;
    cJSON_bool has_pi = 0;
    cJSON_bool enabled_flag = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();

    // step 3: Core operations
    added_pi = cJSON_AddNumberToObject(root, "pi", 3.1415);
    created_hundred = cJSON_CreateNumber(100.0);
    cJSON_AddItemToObject(root, "hundred", created_hundred);
    cJSON_AddItemToObject(root, "enabled", cJSON_CreateTrue());
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    got_pi = cJSON_GetObjectItem(root, "pi");
    pi_val = cJSON_GetNumberValue(got_pi);
    got_hundred = cJSON_GetObjectItem(root, "hundred");
    hundred_val = cJSON_GetNumberValue(got_hundred);
    has_pi = cJSON_HasObjectItem(root, "pi");
    got_enabled = cJSON_GetObjectItem(root, "enabled");
    enabled_flag = cJSON_IsTrue(got_enabled);
    validation ^= (int)(root != NULL);
    validation ^= (int)(added_pi != NULL);
    validation ^= (int)(created_hundred != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(got_pi != NULL);
    validation ^= (int)(got_hundred != NULL);
    validation ^= (int)(got_enabled != NULL);
    validation ^= (int)(pi_val == 3.1415);
    validation ^= (int)(hundred_val == 100.0);
    validation ^= (int)(has_pi != 0);
    validation ^= (int)(enabled_flag != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}