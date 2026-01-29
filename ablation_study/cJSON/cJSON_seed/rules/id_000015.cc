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
//<ID> 15
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *str1 = cJSON_CreateString("alpha");
    cJSON *str2 = cJSON_CreateString("beta");
    cJSON *num_item = NULL;
    cJSON *retrieved_num = NULL;
    double pi = 3.14159;
    double fetched = 0.0;
    char *printed = NULL;

    // step 2: Setup
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, str2);
    cJSON_AddItemToObject(root, "letters", arr);
    num_item = cJSON_AddNumberToObject(root, "pi", pi);

    // step 3: Operate/Validate
    retrieved_num = cJSON_GetObjectItem(root, "pi");
    fetched = cJSON_GetNumberValue(retrieved_num);
    cJSON_AddNumberToObject(root, "pi_scaled", fetched * 2.0);

    // step 4: Cleanup
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}