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
//<ID> 104
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "name", "synth");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *num = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToArray(arr, num);

    // step 2: Configure
    cJSON *flag = cJSON_AddTrueToObject(root, "enabled");
    cJSON *first = cJSON_CreateString("first");
    cJSON_AddItemToArray(arr, first);
    cJSON *inserted = cJSON_CreateString("inserted");
    cJSON_bool insert_ok = cJSON_InsertItemInArray(arr, 1, inserted);

    // step 3: Operate and Validate
    char json_buf[] = "{ \"a\" : 1, \"b\" : [ 1, 2, 3 ] }";
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);
    char *printed = cJSON_Print(root);
    char tmpbuf[128];
    memset(tmpbuf, 0, sizeof(tmpbuf));
    int summary = 0;
    summary += (int)cJSON_IsArray(arr);
    summary += (int)cJSON_IsTrue(flag);
    summary += (printed ? (int)printed[0] : 0);
    summary += (int)insert_ok;
    summary += (int)cJSON_IsObject(parsed);
    (void)tmpbuf;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}