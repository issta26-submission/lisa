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
//<ID> 1069
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *num = (cJSON *)0;
    cJSON *status = (cJSON *)0;
    cJSON *fetched_num = (cJSON *)0;
    cJSON *fetched_status = (cJSON *)0;
    const char *status_value = (const char *)0;
    char *printed = (char *)0;
    double num_value = 0.0;
    cJSON_bool add_ok_child = (cJSON_bool)0;
    cJSON_bool add_ok_num = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object, a child object, a number and a string
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    num = cJSON_CreateNumber(3.14159);
    status = cJSON_CreateString("active");

    // step 3: Configure - attach items using both AddItemToObjectCS and AddItemToObject
    add_ok_child = cJSON_AddItemToObjectCS(root, "child", child);
    add_ok_num = cJSON_AddItemToObject(root, "pi", num);
    cJSON_AddItemToObject(child, "status", status);

    // step 4: Operate - fetch the items, read values and print JSON unformatted
    fetched_num = cJSON_GetObjectItem(root, "pi");
    fetched_status = cJSON_GetObjectItem(root, "child");
    if (fetched_status) fetched_status = cJSON_GetObjectItem(fetched_status, "status");
    num_value = cJSON_GetNumberValue(fetched_num);
    status_value = cJSON_GetStringValue(fetched_status);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);

    // step 5: Validate - compute a small validation score using the results
    validation_score = (int)(root != (cJSON *)0) + (int)(child != (cJSON *)0) + (int)(num != (cJSON *)0) + (int)(status != (cJSON *)0) + (int)(add_ok_child) + (int)(add_ok_num) + (int)(num_value > 3.0) + (int)(status_value != (const char *)0);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}