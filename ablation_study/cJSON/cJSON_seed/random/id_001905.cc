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
//<ID> 1905
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
    cJSON *got_pi = NULL;
    cJSON *got_answer = NULL;
    cJSON *got_enabled = NULL;
    char *out = NULL;
    double pi_value = 0.0;
    double answer_value = 0.0;
    cJSON_bool is_enabled = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "pi", 3.1415);
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", num_item);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Core operations
    got_pi = cJSON_GetObjectItem(root, "pi");
    pi_value = cJSON_GetNumberValue(got_pi);
    got_answer = cJSON_GetObjectItem(root, "answer");
    answer_value = cJSON_GetNumberValue(got_answer);
    got_enabled = cJSON_GetObjectItem(root, "enabled");
    is_enabled = cJSON_IsTrue(got_enabled);
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(got_pi != NULL);
    validation ^= (int)(got_answer != NULL);
    validation ^= (int)(got_enabled != NULL);
    validation ^= (int)(pi_value > 3.0);
    validation ^= (int)(answer_value == 42.0);
    validation ^= (int)(is_enabled != 0);
    validation ^= (int)(out != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}