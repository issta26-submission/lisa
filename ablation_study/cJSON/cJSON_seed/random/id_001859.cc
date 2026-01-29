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
//<ID> 1859
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
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *bool_false = NULL;
    cJSON *retrieved = NULL;
    char *out = NULL;
    double val_b = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num1 = cJSON_CreateNumber(1.23);
    num2 = cJSON_CreateNumber(4.56);
    bool_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "alpha", num1);
    cJSON_AddItemToObject(root, "b", num2);
    cJSON_AddItemToObject(root, "enabled", bool_false);

    // step 3: Core operations
    retrieved = cJSON_GetObjectItem(root, "b");
    val_b = cJSON_GetNumberValue(retrieved);
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(bool_false != NULL);
    validation ^= (int)(retrieved != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(val_b == 4.56);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}