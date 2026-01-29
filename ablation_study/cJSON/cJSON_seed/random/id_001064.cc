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
//<ID> 1064
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *count_num = (cJSON *)0;
    cJSON *ver_str = (cJSON *)0;
    cJSON *fetched_count = (cJSON *)0;
    cJSON *fetched_ver = (cJSON *)0;
    cJSON_bool add_ok_arr = (cJSON_bool)0;
    cJSON_bool add_ok_ver = (cJSON_bool)0;
    double count_value = 0.0;
    const char *version_value = (const char *)0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root, an array, numbers, a count and a version string
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.0);
    num2 = cJSON_CreateNumber(2.0);
    count_num = cJSON_CreateNumber(42.0);
    ver_str = cJSON_CreateString("v1.0");

    // step 3: Configure - populate array and attach items to root using both add APIs
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    add_ok_arr = cJSON_AddItemToObject(root, "values", arr);
    add_ok_ver = cJSON_AddItemToObjectCS(root, "version", ver_str);
    cJSON_AddItemToObject(root, "count", count_num);

    // step 4: Operate - fetch items, read numeric and string values, print and free JSON text
    fetched_count = cJSON_GetObjectItem(root, "count");
    count_value = cJSON_GetNumberValue(fetched_count);
    fetched_ver = cJSON_GetObjectItem(root, "version");
    version_value = cJSON_GetStringValue(fetched_ver);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);

    // step 5: Validate - compute a small validation score using the results
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(add_ok_arr) + (int)(add_ok_ver) + (int)(count_value == 42.0) + (int)(version_value != (const char *)0);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}