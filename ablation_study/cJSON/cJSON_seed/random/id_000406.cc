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
//<ID> 406
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
    cJSON *num_item = (cJSON *)0;
    cJSON *got = (cJSON *)0;
    double extracted = 0.0;
    char json_buf[] = "  {  \"name\"  :  \"example\" , \"list\" : [ 10 , 20 ]  }  ";

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(10.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 4: Operate
    got = cJSON_GetArrayItem(arr, 0);
    extracted = cJSON_GetNumberValue(got);
    cJSON_Minify(json_buf);

    // step 5: Validate
    (void)extracted;
    (void)json_buf;
    (void)got;

    // step 6: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "enabled");
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}