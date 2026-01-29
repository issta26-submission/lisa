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
//<ID> 539
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *outerArr = (cJSON *)0;
    cJSON *doubleArr = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *title_item = (cJSON *)0;
    cJSON *retrieved_title = (cJSON *)0;
    double numbers[3] = {1.1, 2.2, 3.3};
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    cJSON_bool is_raw = 0;
    char *title_val = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    outerArr = cJSON_CreateArray();
    doubleArr = cJSON_CreateDoubleArray(numbers, 3);
    raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    title_item = cJSON_CreateString("example-dataset");

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(outerArr, doubleArr);
    add_ok2 = cJSON_AddItemToObject(root, "data", outerArr);
    add_ok3 = cJSON_AddItemToObject(root, "raw", raw_item);
    cJSON_AddItemToObject(root, "title", title_item);

    // step 4: Operate
    retrieved_title = cJSON_GetObjectItem(root, "title");
    title_val = cJSON_GetStringValue(retrieved_title);
    is_raw = cJSON_IsRaw(cJSON_GetObjectItem(root, "raw"));

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)title_val;
    (void)is_raw;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}