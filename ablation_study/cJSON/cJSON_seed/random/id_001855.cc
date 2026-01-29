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
//<ID> 1855
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
    cJSON *nested = NULL;
    cJSON *num_answer = NULL;
    cJSON *num_pi = NULL;
    cJSON *flag_false = NULL;
    cJSON *retrieved = NULL;
    char *out = NULL;
    double retrieved_value = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_answer = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", num_answer);
    flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag_false);
    nested = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", nested);
    num_pi = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(nested, "pi", num_pi);

    // step 3: Core operations
    retrieved = cJSON_GetObjectItem(root, "answer");
    retrieved_value = cJSON_GetNumberValue(retrieved);
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(nested != NULL);
    validation ^= (int)(num_answer != NULL);
    validation ^= (int)(num_pi != NULL);
    validation ^= (int)(flag_false != NULL);
    validation ^= (int)(retrieved != NULL);
    validation ^= (int)(retrieved_value == 42.0);
    validation ^= (int)(out != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}