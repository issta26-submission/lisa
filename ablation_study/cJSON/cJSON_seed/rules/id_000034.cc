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
//<ID> 34
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
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *num_item = NULL;
    cJSON *retrieved_count = NULL;
    cJSON *retrieved_items = NULL;
    cJSON *second_string = NULL;
    char *sval = NULL;
    double count_val = 0.0;
    double derived = 0.0;
    char *printed = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    num_item = cJSON_AddNumberToObject(root, "count", 2.0);

    // step 3: Operate / Validate
    retrieved_count = cJSON_GetObjectItem(root, "count");
    count_val = cJSON_GetNumberValue(retrieved_count);
    derived = count_val + (double)cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "derived", derived);
    retrieved_items = cJSON_GetObjectItem(root, "items");
    second_string = cJSON_GetArrayItem(retrieved_items, 1);
    sval = cJSON_GetStringValue(second_string);
    derived += (double)(unsigned char)sval[0];
    cJSON_AddNumberToObject(root, "final", derived);

    // step 4: Cleanup
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}