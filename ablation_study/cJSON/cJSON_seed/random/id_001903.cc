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
//<ID> 1903
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
    cJSON *num_item2 = NULL;
    cJSON *bool_item = NULL;
    cJSON *got_num1 = NULL;
    cJSON *got_num2 = NULL;
    cJSON *got_bool = NULL;
    char *out = NULL;
    double v1 = 0.0;
    double v2 = 0.0;
    cJSON_bool is_true = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_item = cJSON_AddNumberToObject(root, "pi", 3.14159);
    num_item2 = cJSON_CreateNumber(2.71828);
    cJSON_AddItemToObject(root, "e", num_item2);
    bool_item = cJSON_AddBoolToObject(root, "enabled", (cJSON_bool)1);

    // step 3: Core operations
    got_num1 = cJSON_GetObjectItem(root, "pi");
    v1 = cJSON_GetNumberValue(got_num1);
    got_num2 = cJSON_GetObjectItem(root, "e");
    v2 = cJSON_GetNumberValue(got_num2);
    got_bool = cJSON_GetObjectItem(root, "enabled");
    is_true = cJSON_IsTrue(got_bool);
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(num_item2 != NULL);
    validation ^= (int)(bool_item != NULL);
    validation ^= (int)(got_num1 != NULL);
    validation ^= (int)(got_num2 != NULL);
    validation ^= (int)(got_bool != NULL);
    validation ^= (int)(cJSON_IsNumber(got_num1));
    validation ^= (int)(cJSON_IsNumber(got_num2));
    validation ^= (int)(v1 > 3.0);
    validation ^= (int)(v2 > 2.0);
    validation ^= (int)(is_true != 0);
    validation ^= (int)(out != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}